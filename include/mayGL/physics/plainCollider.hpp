#ifndef plainCollider_hpp
#define plainCollider_hpp

#include "colliderComponent.hpp"
#include "../support/support.hpp"

namespace mayGL
{
    namespace physics
    {
        class PlainCollider : public Collider
        {
        public:

            PlainCollider(entity::Entity *t_parent, std::string t_id, std::string t_physicsBodyId, std::string t_transformId, geometry::plain t_p);
            ~PlainCollider() override = default;

            glm::vec3 closestPointToPlain(glm::vec3 t_point);
            float smallestDistToPoint(glm::vec3 t_point);
            bool pointInPlain(glm::vec3 t_point);

            inline geometry::plain getPlain() { return m_plain; }
            inline glm::vec3 getNormal() { return m_normal; }

            bool testCollision(Collider* t_other, Collision &t_m) override;

            void update() override {}

        private:

            geometry::plain m_plain;
            glm::vec3 m_normal;
        };
    }
}

#endif