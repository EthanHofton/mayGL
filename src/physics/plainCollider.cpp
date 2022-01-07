#include <mayGL/physics/plainCollider.hpp>
#include <mayGL/physics/collisions.hpp>
#include <mayGL/entity/entity.hpp>

namespace may
{
    namespace physics
    {
        PlainCollider::PlainCollider(entity::Entity *t_parent, std::string t_id, std::string t_physicsBodyId, std::string t_transformId, geometry::plain t_p) : Collider(t_parent, t_id, plain, t_physicsBodyId, t_transformId)
        {
            m_plain = t_p;

            m_normal = geometry::calculatePlainNormal(m_plain);

            m_colliderIndices = {
                0,1,
                1,2,
                2,3,
                3,0  
            };

            getColliderMesh()->setVertices((void*)&m_plain.m_a, sizeof(colliderVertex)*4);
            getColliderMesh()->setIndices((void*)&m_colliderIndices[0], sizeof(unsigned int)*m_colliderIndices.size());
        }

        bool PlainCollider::testCollision(Collider* t_other, Collision &t_m)
        {
            switch (t_other->getColliderType())
            {
            case sphere:
                return plainVsSphere(this, static_cast<SphereCollider*>(t_other), t_m);
            case cube:
                return plainVsCube(this, static_cast<CubeCollider*>(t_other), t_m);
            case plain:
                return plainVsPlain(this, static_cast<PlainCollider*>(t_other), t_m);
            default:
                return {};
            }
        }

        glm::vec3 PlainCollider::closestPointToPlain(glm::vec3 t_point)
        {
            float dist = glm::dot((t_point - m_plain.m_a), m_normal);

            return t_point + -(m_normal * dist);
        }

        float PlainCollider::smallestDistToPoint(glm::vec3 t_point)
        {
            return glm::dot((t_point - m_plain.m_a), m_normal);
        }

        bool PlainCollider::pointInPlain(glm::vec3 t_point)
        {
            glm::vec3 p1 = m_plain.m_a;
            glm::vec3 p2 = m_plain.m_b;
            glm::vec3 p4 = m_plain.m_d;

            glm::vec3 i = p2 - p1;
            glm::vec3 j = p4 - p1;
            glm::vec3 v = t_point - p1;


            if (0 < glm::dot(v, i) && glm::dot(v, i) < glm::dot(i, i))
            {
                if (0 < glm::dot(v, j) && glm::dot(v, j) < glm::dot(j, j))
                {
                    return true;
                }
            }

            return false;
        }
    }
}