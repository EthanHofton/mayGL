#ifndef physicsBody_hpp
#define physicsBody_hpp

#include "../component/transformComponent.hpp"
#include <glm/glm.hpp>

namespace mayGL
{
    namespace physics
    {
        class PhysicsBody : public component::Component
        {
        public:

            PhysicsBody(entity::Entity *t_parent, std::string t_id);
            ~PhysicsBody() = default;

            inline glm::vec3 getForce() { return m_force; }
            void addForce(glm::vec3 t_dforce);
            void setForce(glm::vec3 t_force);

            inline glm::vec3 getVelocity() { return m_velocity; }
            void setVelocity(glm::vec3 t_velocity);
            void addVelocity(glm::vec3 t_dvelocity);

            void setMass(float t_mass);
            inline float getMass() { return m_mass; }
            inline float getInvMass() { return m_invMass; }

            void setGravityScale(float t_gravityScale);
            inline float getGravityScale() { return m_gravityScale; }

            inline float getRestitution() { return m_restitution; }
            inline void setRestitution(float t_restitution) { m_restitution = t_restitution; }

            inline float getStaticFriction() { return m_staticFriction; }
            inline void setStaticFriction(float t_staticFriction) { m_staticFriction = t_staticFriction; }

            inline float getDynamicFriction() { return m_dynamicFriction; }
            inline void setDynamicFriction(float t_dynamicFriction) { m_dynamicFriction = t_dynamicFriction; }

            inline std::vector<component::Transform*> getTransforms() { return m_transforms; }
            void addTransform(std::string t_transformId);
            void addTransforms(std::vector<std::string> t_transformIds);
            void addAllTransforms();
            void removeTransform(std::string t_transformId);
            void removeAllTransforms();

            void update() override {}

            void imguiComponentInspector() override;

        private:

            // imgui
            bool m_dragEdit;
            glm::vec3 m_imguiDf;
            glm::vec3 m_imguiDv;
            // imgui

            glm::vec3 m_velocity;
            glm::vec3 m_force;

            float m_mass;
            float m_invMass;
            float m_gravityScale;
            float m_restitution;
            float m_staticFriction;
            float m_dynamicFriction;

            std::vector<component::Transform*> m_transforms;
        };
    }
}

#endif