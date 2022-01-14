#ifndef entity_hpp
#define entity_hpp

#include <iostream>
#include <vector>
#include <map>

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
            
            void updateComponents();
            inline virtual void update() {}
            
            // -- commonly used functions
            inline bool hasMeshComponent() { return hasComponent(component::mesh); }
            inline bool hasTransformComponent() { return hasComponent(component::transform); }
            inline bool hasTextureComponent() { return hasComponent(component::texture); }
            inline bool hasTextureAtlasComponent() { return hasComponent(component::textureAtlas); }
            inline bool hasColorComponent() { return hasComponent(component::color); }
            inline bool hasPhysicsBody() { return hasComponent(component::physicsBody); }
            inline bool hasCollider() { return hasComponent(component::collider); }
            
            inline component::Transform *getTransformComponent(std::string t_id) 
            { return getComponentType<component::Transform*, component::transform>(t_id); }
            std::vector<component::Transform*> getTransformComponents()
            { return getComponents<component::Transform*, component::transform>(); }

            component::Mesh *getMeshComponent(std::string t_id)
            { return getComponentType<component::Mesh*, component::mesh>(t_id); }
            std::vector<component::Mesh*> getMeshComponents()
            { return getComponents<component::Mesh*, component::mesh>(); }

            component::Texture *getTextureComponent(std::string t_id)
            { return getComponentType<component::Texture*, component::texture>(t_id); }
            std::vector<component::Texture*> getTextureComponents()
            { return getComponents<component::Texture*, component::texture>(); }

            component::TextureAtlas *getTextureAtlasComponent(std::string t_id)
            { return getComponentType<component::TextureAtlas*, component::textureAtlas>(t_id); }
            std::vector<component::TextureAtlas*> getTextureAtlasComponents()
            { return getComponents<component::TextureAtlas*, component::textureAtlas>(); }

            component::ColorComponent *getColorComponent(std::string t_id)
            { return getComponentType<component::ColorComponent*, component::color>(t_id); }
            std::vector<component::ColorComponent*> getColorComponents()
            { return getComponents<component::ColorComponent*, component::color>(); }

            physics::PhysicsBody *getPhysicsBody(std::string t_id)
            { return getComponentType<physics::PhysicsBody*, component::physicsBody>(t_id); }
            std::vector<physics::PhysicsBody*> getPhysicsBodys()
            { return getComponents<physics::PhysicsBody*, component::physicsBody>(); }

            physics::Collider *getCollider(std::string t_id)
            { return getComponentType<physics::Collider*, component::collider>(t_id); }
            std::vector<physics::Collider*> getColliders()
            { return getComponents<physics::Collider*, component::collider>(); }

            physics::CubeCollider *getCubeCollider(std::string t_id)
            { return getComponentType<physics::CubeCollider*, component::collider>(t_id); }
            std::vector<physics::CubeCollider*> getCubeColliders()
            { return getColliderComponents<physics::CubeCollider*, physics::cube>(); }

            physics::SphereCollider *getSphereCollider(std::string t_id)
            { return getComponentType<physics::SphereCollider*, component::collider>(t_id); }
            std::vector<physics::SphereCollider*> getSphereColliders()
            { return getColliderComponents<physics::SphereCollider*, physics::sphere>(); }

            physics::PlainCollider *getPlainCollider(std::string t_id)
            { return getComponentType<physics::PlainCollider*, component::collider>(t_id); }
            std::vector<physics::PlainCollider*> getPlainColliders()
            { return getColliderComponents<physics::PlainCollider*, physics::plain>(); }
            // -- commonly used functions
            
        private:
            
            inline void child(Entity *t_e) { m_children.push_back(t_e); }

            template<typename T, component::component_types T_TYPE>
            inline const T getComponentType(const std::string& t_id)
            {
                if (!hasComponent(T_TYPE))
                {
                    CORE_ERROR("entity with id '{}' component not found with id {}", m_id, t_id);
                    return nullptr;
                }

                return static_cast<T>(findComponent(t_id));
            }

            template<typename T, component::component_types T_TYPE>
            inline const std::vector<T> getComponents()
            {
                std::vector<T> returnComponents;
                
                for (auto component : m_components)
                {
                    if (component->getType() == T_TYPE)
                    {
                        returnComponents.push_back(static_cast<T>(component));
                    }
                }
                
                return returnComponents;
            }

            template<typename T, physics::colliderTypes T_COLLIDER_TYPE>
            inline const std::vector<T> getColliderComponents()
            {
                if (!hasComponent(component::collider))
                {
                    CORE_ERROR("entity with id '{}' no components found with type 'collider", m_id);
                    return {};
                }

                std::vector<T> returnComponents;
                
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
            
        private:
            
            bool m_active;
            std::vector<component::Component *> m_components;
            
            Entity *m_parent;
            std::vector<Entity*> m_children;
            std::string m_id;
            
            static int s_entityCount;
            static std::map<std::string, Entity*> s_allEntites;            
        };
    }
}

#endif