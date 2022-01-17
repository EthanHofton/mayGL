#ifndef entity_hpp
#define entity_hpp

#include <iostream>
#include <vector>
#include <map>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "../core/logger.hpp"
#include "../component/component.hpp"
#include "../component/meshComponent.hpp"
#include "../component/transformComponent.hpp"
#include "../component/textureComponent.hpp"
#include "../component/textureAtlasComponent.hpp"
#include "../component/colorComponent.hpp"
#include "../physics/physicsBody.hpp"
#include "../physics/sphereCollider.hpp"
#include "../physics/cubeCollider.hpp"
#include "../physics/plainCollider.hpp"

namespace mayGL
{
    enum memory_types
    {
        strong,
        weak
    };

    namespace entity
    {
        class Entity
        {
        public:
            
            inline static Entity* getEntity(std::string t_id) { return s_allEntites[t_id]; }
            static void deleteAllEntities();
            
            Entity(std::string t_id = "");
            virtual ~Entity();
            
            void active(bool t_a);
            bool isActive();
            
            void parent(Entity *t_e);
            inline Entity *getParent() { return m_parent; }
            inline bool hasParent() { return (m_parent != nullptr); }
            
            inline std::vector<Entity*> getChildren() { return m_children; }
            inline bool hasChildren() { return (!m_children.empty()); }
            
            void entityId(std::string t_id);
            inline std::string getEntityId() { return m_id; }
            
            void addComponent(component::Component* t_component);
            component::Component *findComponent(std::string t_id);
            std::vector<component::Component*> findComponentByType(component::component_types t_type);
            void removeComponent(std::string t_id, memory_types t_memoryType = strong);
            bool hasComponent(component::component_types t_type);
            bool hasComponent(std::string t_id);

            inline void shouldShowImguiInspector(bool t_b) { m_showImguiInspector = t_b; }
            inline bool showingImguiInspector() { return m_showImguiInspector; }
            
            void updateComponents();
            inline virtual void update() {}

            void imguiInspector(std::vector<Entity*> t_entites);
            virtual void customImguiProperties() {}
            
        private:
            
            inline void child(Entity *t_e) { m_children.push_back(t_e); }
            void removeChild(std::string t_id);

        public:

            template<typename T, component::component_types T_TYPE>
            inline T* getComponent(const std::string& t_id)
            {
                if (!hasComponent(T_TYPE))
                {
                    CORE_ERROR("entity with id '{}' component not found with id {}", m_id, t_id);
                    return nullptr;
                }

                return static_cast<T*>(findComponent(t_id));
            }

            template<typename T>
            inline T* getComponent(const std::string& t_id)
            {
                // no type error checking
                return static_cast<T*>(findComponent(t_id));
            }

            template<typename T, component::component_types T_TYPE>
            inline std::vector<T*> getComponents()
            {
                std::vector<T*> returnComponents;
                
                for (auto component : m_components)
                {
                    if (component->getType() == T_TYPE)
                    {
                        returnComponents.push_back(static_cast<T*>(component));
                    }
                }
                
                return returnComponents;
            }

            template<typename T, physics::colliderTypes T_COLLIDER_TYPE>
            inline std::vector<T*> getColliderComponents()
            {
                if (!hasComponent(component::collider))
                {
                    CORE_ERROR("entity with id '{}' no components found with type 'collider", m_id);
                    return {};
                }

                std::vector<T*> returnComponents;
                
                for (auto component : m_components)
                {
                    if (component->getType() == component::collider)
                    {
                        auto collider = static_cast<physics::Collider*>(component);
                        if (collider->getColliderType() == T_COLLIDER_TYPE)
                        {
                            returnComponents.push_back(static_cast<T>(component));
                        }
                        
                    }
                }
                
                return returnComponents;
            }

            template<typename T>
            T* as()
            {
                return static_cast<T*>(this);
            }

        private:
            
            bool m_active;
            std::vector<component::Component *> m_components;
            
            Entity *m_parent;
            std::vector<Entity*> m_children;
            std::string m_id;
            int m_entityUID;
            
            static int s_entityCount;
            static std::map<std::string, Entity*> s_allEntites;

            bool m_showImguiInspector;     
        };
    }
}

#endif