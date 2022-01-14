#ifndef colliderComponent_hpp
#define colliderComponent_hpp

#include "../component/component.hpp"
#include "../component/meshComponent.hpp"
#include "collisionManifold.hpp"
#include "physicsBody.hpp"
#include "../component/transformComponent.hpp"

namespace mayGL
{
    namespace physics
    {
        enum colliderTypes
        {
            sphere,
            cube,
            plain,
            polygon,
            mesh // maybe
        };

        struct colliderVertex
        {
            glm::vec3 m_pos;
        };

        class Collider : public component::Component
        {
        public:

            Collider(entity::Entity *t_parent, std::string t_id, colliderTypes t_colliderType, std::string t_physicsBodyId, std::string t_transformId);
            virtual ~Collider() = default;

            inline bool shouldShowCollider() { return m_showCollider; }
            void showCollider(bool t_showCollider);

            inline colliderTypes getColliderType() { return m_colliderType; }
            inline component::Mesh* getColliderMesh() { return m_colliderMesh; }

            inline PhysicsBody *getPhysicsBody() { return m_physicsBody; }
            inline component::Transform *getTransform() { return m_transform; }

            virtual bool testCollision(Collider* t_other, Collision &t_m);

        protected:

            std::vector<colliderVertex> m_colliderVertices;
            std::vector<unsigned int> m_colliderIndices;

        private:

            bool m_showCollider;
            colliderTypes m_colliderType;

            component::Mesh *m_colliderMesh;

            PhysicsBody *m_physicsBody;
            component::Transform *m_transform;

        };
    }
}

#endif