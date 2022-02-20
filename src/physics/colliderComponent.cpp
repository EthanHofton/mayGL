#include <mayGL/physics/colliderComponent.hpp>
#include <mayGL/entity/entity.hpp>
#include <mayGL/physics/collisions.hpp>

namespace mayGL
{
    namespace physics
    {
        Collider::Collider(entity::Entity *t_parent, std::string t_id, colliderTypes t_colliderType, std::string t_physicsBodyId, std::string t_transformId) : component::Component(t_parent, component::collider, t_id)
        {
            m_transform = getParent()->getComponent<component::Transform, component::transform>(t_transformId);
            m_physicsBody = getParent()->getComponent<PhysicsBody, component::physicsBody>(t_physicsBodyId);
            m_colliderType = t_colliderType;
            m_showCollider = false;

            vertex::VertexLayout *vLayout = new vertex::VertexLayout;
            vLayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(colliderVertex, m_pos)));

            m_colliderMesh = new component::Mesh(t_parent, "collider_mesh", vLayout);
            m_colliderMesh->visable(m_showCollider);

            getParent()->addComponent(m_colliderMesh);
            m_transform->addMesh(m_colliderMesh->getId());
        }

        void Collider::showCollider(bool t_showCollider)
        {
            m_showCollider = t_showCollider;
            m_colliderMesh->visable(m_showCollider);
        }

        bool Collider::testCollision(Collider* t_other, Collision &t_m)
        {
            if (m_colliderType == sphere && t_other->getColliderType() == sphere)
            {
                return sphereVsSphere(static_cast<SphereCollider*>(this), static_cast<SphereCollider*>(t_other), t_m);
            } else if (m_colliderType == sphere && t_other->getColliderType() == cube) {
                return sphereVsCube(static_cast<SphereCollider*>(this), static_cast<CubeCollider*>(t_other), t_m);
            } else if (m_colliderType == cube && t_other->getColliderType() == cube) {
                return cubeVsCube(static_cast<CubeCollider*>(this), static_cast<CubeCollider*>(t_other), t_m);
            } else if (m_colliderType == cube && t_other->getColliderType() == sphere) {
                return cubeVsSphere(static_cast<CubeCollider*>(this), static_cast<SphereCollider*>(t_other), t_m);
            } else if (m_colliderType == plain && t_other->getColliderType() == sphere) {
                return cubeVsSphere(static_cast<CubeCollider*>(this), static_cast<SphereCollider*>(t_other), t_m);
            }

            return false;
        }
    }
}