#ifndef physicsWorld_hpp
#define physicsWorld_hpp

#include <vector>
#include <algorithm>

#include "../entity/entity.hpp"
#include "../core/timer.hpp"
#include "physicsBody.hpp"
#include "collisionManifold.hpp"

namespace mayGL
{
    namespace physics
    {
        class PhysicsWorld : public entity::Entity
        {
        public:

            PhysicsWorld();
            ~PhysicsWorld() = default;

            void addPhysicsEntity(entity::Entity *t_enitiy) { m_physicsEntites.push_back(t_enitiy); }
            void removePhysicsEntity(entity::Entity *t_enitiy);
            void removePhysicsEntity(std::string t_enitiyId);
            void intergrate(PhysicsBody *t_body);
            void resolve(Collision &t_c);
            void positionalCorrection(Collision &t_c);

            inline glm::vec3 getGravity() { return m_gravity; }
            inline void setGravity(glm::vec3 t_gravity) { m_gravity = t_gravity; }

            inline bool isGravityAffected() { return m_gravityAffected; }
            inline void GravityAffected(bool t_g) { m_gravityAffected = t_g; }

            void update() override;

        private:

            core::Timer *m_timer;
            std::vector<Entity*> m_physicsEntites;
            float m_t;

            glm::vec3 m_gravity;
            bool m_gravityAffected;
        };
    }
}

#endif
