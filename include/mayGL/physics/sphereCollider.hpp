#ifndef sphereCollider_hpp
#define sphereCollider_hpp

#include "colliderComponent.hpp"
#include "../support/support.hpp"

namespace mayGL
{
    namespace physics
    {
        class SphereCollider : public Collider
        {
        public:

            SphereCollider(entity::Entity *t_parent, std::string t_id, std::string t_physicsBodyId, std::string t_transformId, float t_radius);
            ~SphereCollider() override = default;

            glm::vec3 closesPointToSphere(glm::vec3 t_point);

            inline float getRadius() { return m_radius; }

            bool testCollision(Collider* t_other, Collision &t_m) override;

            void update() override {}

        private:

            void setMeshVertices();

        private:

            float m_radius;
        };
    }
}

#endif