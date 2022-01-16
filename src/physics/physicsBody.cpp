#include <mayGL/physics/physicsBody.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace physics
    {
        PhysicsBody::PhysicsBody(entity::Entity *t_parent, std::string t_id) : component::Component(t_parent, component::physicsBody, t_id)
        {
            m_force = glm::vec3(0.0f);
            m_velocity = glm::vec3(0.0f);
            m_mass = 1.0f;
            m_invMass = 1.0f;
            m_gravityScale = 1.0f;
            m_restitution = 1.0f;
            m_staticFriction = 0.0f;
            m_dynamicFriction = 0.0f;
        }

        void PhysicsBody::addForce(glm::vec3 t_dforce)
        {
            m_force += t_dforce;
        }

        void PhysicsBody::setForce(glm::vec3 t_force)
        {
            m_force = t_force;
        }

        void PhysicsBody::setVelocity(glm::vec3 t_velocity)
        {
            m_velocity = t_velocity;
        }

        void PhysicsBody::addVelocity(glm::vec3 t_dvelocity)
        {
            m_velocity += t_dvelocity;
        }

        void PhysicsBody::setMass(float t_mass)
        {
            m_mass = t_mass;
            m_invMass = (m_mass == 0) ? 0 : (1.0f / m_mass);
        }

        void PhysicsBody::setGravityScale(float t_gravityScale)
        {
            m_gravityScale = t_gravityScale;
        }

        void PhysicsBody::addTransform(std::string t_transformId)
        {
            m_transforms.push_back(getParent()->getComponent<component::Transform, component::transform>(t_transformId));
        }

        void PhysicsBody::addTransforms(std::vector<std::string> t_transformIds)
        {
            for (auto transformId : t_transformIds)
            {
                addTransform(transformId);
            }
        }

        void PhysicsBody::addAllTransforms()
        {
            m_transforms.clear();
            m_transforms = getParent()->getComponents<component::Transform, component::transform>();
        }

        void PhysicsBody::removeTransform(std::string t_transformId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_transforms.size(); index++)
            {
                if (m_transforms[index]->getId() == t_transformId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                m_transforms.erase(m_transforms.begin() + index);
            }
        }

        void PhysicsBody::removeAllTransforms()
        {
            m_transforms.clear();
        }
    }
}