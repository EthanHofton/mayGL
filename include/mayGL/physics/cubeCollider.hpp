#ifndef cubeCollider_hpp
#define cubeCollider_hpp

#include "colliderComponent.hpp"

namespace may
{
    namespace physics
    {
        class CubeCollider : public Collider
        {
        public:

            CubeCollider(entity::Entity *t_parent, std::string t_id, std::string t_physicsBodyId, std::string t_transformId, glm::vec3 t_size);
            ~CubeCollider() override = default;

            bool testCollision(Collider* t_other, Collision &t_m) override;

            glm::vec3 getClosesPointToCube(glm::vec3 t_point);

            inline glm::vec3 getSize() { return m_size; }
            inline glm::vec3 getMin() { return m_min; }
            inline glm::vec3 getMax() { return m_max; }

            void update() override {}
        
        private:

            void setMeshVertices();

        private:

            glm::vec3 m_size;
            glm::vec3 m_min;
            glm::vec3 m_max;

        };
    }
}

#endif