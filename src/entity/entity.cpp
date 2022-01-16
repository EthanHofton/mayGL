#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace entity
    {
        std::map<std::string, Entity*> Entity::s_allEntites;
        int Entity::s_entityCount = 0;
        
        Entity::Entity(std::string t_id)
        {
            if (t_id == "")
            {
                m_id = "entity" + std::to_string(s_entityCount);
            } else {
                m_id = t_id;
            }
            
            m_active = true;
            m_parent = nullptr;
            s_allEntites[m_id] = this;
            m_entityUID = s_entityCount;
            s_entityCount++;
            m_showImguiInspector = false;
            
            CORE_INFO("entity with id '{0}' created", m_id);
            CORE_DEBUG("all entites list size: {0}", s_allEntites.size());
        }
        
        Entity::~Entity()
        {
            auto instance = s_allEntites.find(m_id);
            if (instance != s_allEntites.end())
            {
                s_allEntites.erase(instance);
            }
            
            for (auto child : m_children)
            {
                child->parent(nullptr);
            }
            
            CORE_INFO("entity with id '{0}' deleted", m_id);
            CORE_DEBUG("all entites list size: {0}", s_allEntites.size());
            for (auto comp : m_components)
            {
                delete comp;
                comp = nullptr;
            }
        }
        
        void Entity::parent(Entity *t_e)
        {
            m_parent = t_e;
            if (m_parent != nullptr)
            {
                t_e->child(this);
                active(m_parent->isActive());
            }
            std::string parent_id = (t_e == nullptr) ? "nullptr" : t_e->getEntityId();
            CORE_INFO("entity with id '{0}' set parent to entity with id '{1}'", m_id, parent_id);
        }
        
        void Entity::entityId(std::string t_id)
        {
            auto instance = s_allEntites.find(m_id);
            if (instance != s_allEntites.end())
            {
                s_allEntites.erase(instance);
            }
            
            CORE_INFO("entity with id '{0}' renamed to {1}", m_id, t_id);
            
            m_id = t_id;
            s_allEntites[m_id] = this;
        }
        
        void Entity::active(bool t_a)
        {
            m_active = t_a;
            CORE_INFO("entity with id {0} set active to {1}", m_id, t_a);
        }
        
        bool Entity::isActive()
        {
            if (hasParent())
            {
                return getParent()->isActive();
            }
            
            return m_active;
        }
        
        void Entity::deleteAllEntities()
        {
            std::vector<Entity*> entitesToDelete;
            for (auto e : s_allEntites)
            {
                entitesToDelete.push_back(e.second);
            }
            
            for (auto e : entitesToDelete)
            {
                delete e;
                e = nullptr;
            }
            
            s_allEntites.clear();
        }
        
        void Entity::addComponent(component::Component* t_component)
        {
            if (hasComponent(t_component->getId()))
            {
                CORE_CRITICAL("entity with id '{}' allready has component with id '{}'", m_id, t_component->getId());
            }
            assert(hasComponent(t_component->getId()) != true);

            m_components.push_back(t_component);
            CORE_INFO("entity with id '{0}' added component with id {1}", m_id, t_component->getId());
        }
        
        component::Component *Entity::findComponent(std::string t_id)
        {
            component::Component *returnComponent = nullptr;
            
            for (auto component : m_components)
            {
                if (component->getId() == t_id)
                {
                    returnComponent = component;
                    break;
                }
            }

            if (returnComponent == nullptr)
            {
                CORE_WARN("entity with id '{}' does not have component with id '{}'", m_id, t_id);
            }
            
            return returnComponent;
        }
        
        std::vector<component::Component*> Entity::findComponentByType(component::component_types t_type)
        {
            std::vector<component::Component*> returnComponents;
            
            for (auto component : m_components)
            {
                if (component->getType() == t_type)
                {
                    returnComponents.push_back(component);
                }
            }
            
            return returnComponents;
        }
        
        void Entity::removeComponent(std::string t_id, memory_types t_memoryType)
        {
            int index = 0;
            bool found = false;
            for (; index < m_components.size(); index++)
            {
                if (m_components[index]->getId() == t_id)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                CORE_INFO("entity with id '{0}' removing component with id '{1}'", m_id, m_components[index]->getId());
                if (t_memoryType == strong)
                {
                    delete m_components[index];
                    CORE_INFO("entity with id '{0}' deleting component with id '{1}'", m_id, m_components[index]->getId());
                }
                m_components.erase(m_components.begin() + index);
            }
        }
        
        bool Entity::hasComponent(component::component_types t_type)
        {
            for (auto element : m_components)
            {
                if (element->getType() == t_type)
                {
                    return true;
                }
            }
            
            return false;
        }
        
        bool Entity::hasComponent(std::string t_id)
        {
            for (auto element : m_components)
            {
                if (element->getId() == t_id)
                {
                    return true;
                }
            }
            
            return false;
        }
        
        void Entity::updateComponents()
        {
            if (m_active)
            {
                for (auto component : m_components)
                {
                    component->update();
                }
            }
        }

        void Entity::imguiInspector()
        {
            if (m_showImguiInspector)
            {
                std::string windowName = "inspector: "+ m_id +"###" + std::to_string(m_entityUID);
                ImGui::Begin(windowName.c_str(), &m_showImguiInspector);

                ImGui::PushItemWidth(ImGui::GetFontSize() * -10);

                // display entity ID
                std::string idCopy = m_id;
                if (ImGui::InputTextWithHint("Entity ID", "Enter entity id", &idCopy, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    if (idCopy != m_id)
                    {
                        entityId(idCopy);
                    }
                }

                ImGui::LabelText((m_active) ? "True" : "False", "m_active:");
                if (hasParent())
                {
                    ImGui::LabelText("", "m_parent:");
                    ImGui::SameLine();
                    if (ImGui::Button(m_parent->getEntityId().c_str()))
                    {
                        m_parent->shouldShowImguiInspector(true);
                    }
                } else {
                    ImGui::LabelText("nullptr", "m_parent:");
                }

                ImGui::LabelText(std::to_string(m_entityUID).c_str(), "m_entityUID:");

                customImguiProperties();
                
                ImGui::AlignTextToFramePadding();
                bool componetsCollapsed = ImGui::TreeNodeEx("Components", ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                ImGui::SameLine(ImGui::GetWindowWidth() - 65);
                ImGui::Text("%lu", m_components.size());
                if (componetsCollapsed)
                {
                    for (auto component : m_components)
                    {
                        if (ImGui::CollapsingHeader(component->getId().c_str()))
                        {
                            component->imguiComponentInspector();
                        }
                    }
                }

                ImGui::AlignTextToFramePadding();
                bool childrenCollapsed = ImGui::TreeNode("Children");
                ImGui::SameLine(ImGui::GetWindowWidth() - 65);
                ImGui::Text("%lu", m_children.size());
                if (childrenCollapsed)
                {
                    for (int i = 0; i < m_children.size(); i++)
                    {
                        ImGui::TreeNodeEx((int *)(intptr_t)i, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "%s", m_children[i]->getEntityId().c_str());
                        ImGui::SameLine(ImGui::GetWindowWidth() - 85);
                        if (ImGui::SmallButton("Inspect"))
                        {
                            m_children[i]->shouldShowImguiInspector(true);
                        }
                    }
                    ImGui::TreePop();
                }

                ImGui::End();
            }
        }
    }
}