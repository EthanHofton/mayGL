#include <mayGL/core/page.hpp>

namespace mayGL
{
    namespace core
    {
        std::map<std::string, Page*> Page::s_allPages;
        
        Page::Page(Window *t_parent, std::string t_pageId) : m_pageId(t_pageId), m_parent(t_parent)
        {
            CORE_INFO("creating page with id: {0}", t_pageId);
            s_allPages[m_pageId] = this;
        }
        
        Page::~Page()
        {
            auto instance = s_allPages.find(m_pageId);
            if (instance != s_allPages.end())
            {
                s_allPages.erase(instance);
            }
            
            CORE_INFO("deleteing page with id: {0}", m_pageId);
        }
        
        void Page::updateEntities()
        {
            for (auto entity : m_entities)
            {
                if (entity->isActive())
                {
                    entity->updateComponents();
                    entity->update();
                }
            }
        }

        void Page::imguiShowScene()
        {
            std::string pageTitle = m_pageId + ": Scene";
            ImGui::Begin(pageTitle.c_str());

            ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

            for (int i = 0; i < getEntites().size(); i++)
            {
                if (!getEntites()[i]->hasParent())
                {
                    addEntityPlaceholder(getEntites()[i], i);
                }
            }

            ImGui::End();

            for (auto entity : getEntites())
            {
                entity->imguiInspector(getEntites());
            }
        }

        void Page::addEntityPlaceholder(entity::Entity *t_e, int t_c)
        {
            ImGui::PushID(t_c);

            bool nodeOpen = false;
            if (t_e->hasChildren())
            {
                nodeOpen = ImGui::TreeNode((int *)(intptr_t)t_c, "%s", t_e->getEntityId().c_str());
            } else {
                ImGui::TreeNodeEx((int *)(intptr_t)t_c, ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "%s", t_e->getEntityId().c_str());
            }

            ImGui::SameLine(ImGui::GetWindowWidth() - 65);
            if (ImGui::SmallButton("Inspect"))
            {
                t_e->shouldShowImguiInspector(true);
            }

            if (nodeOpen)
            {
                for (int i = 0; i < t_e->getChildren().size(); i++)
                {
                    ImGui::PushID(i);
                    addEntityPlaceholder(t_e->getChildren()[i], i);
                    ImGui::PopID();
                }
                
                ImGui::TreePop();
            } 

            ImGui::PopID();
        }
        
    }
}
