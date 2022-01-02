#include <mayGL/physics/cubeCollider.hpp>
#include <mayGL/physics/collisions.hpp>
#include <mayGL/entity/entity.hpp>

namespace may
{
    namespace physics
    {
        CubeCollider::CubeCollider(entity::Entity *t_parent, std::string t_id, std::string t_physicsBodyId, std::string t_transformId, glm::vec3 t_size) : Collider(t_parent, t_id, cube, t_physicsBodyId, t_transformId)
        {
            m_size = t_size;

            setMeshVertices();

            getColliderMesh()->setVertices((void*)&m_colliderVertices[0], sizeof(colliderVertex)*m_colliderVertices.size());
            getColliderMesh()->setIndices((void*)&m_colliderIndices[0], sizeof(unsigned int)*m_colliderIndices.size());

            m_min = m_colliderVertices[0].m_pos;
            m_max = m_colliderVertices[0].m_pos;

            for (auto vertice : m_colliderVertices)
            {
                m_min = glm::min(vertice.m_pos, m_min);
                m_max = glm::max(vertice.m_pos, m_max);
            }
        }

        bool CubeCollider::testCollision(Collider* t_other, Collision &t_m)
        {
            switch (t_other->getColliderType())
            {
            case sphere:
                return cubeVsSphere(this, static_cast<SphereCollider*>(t_other), t_m);
            case cube:
                return cubeVsCube(this, static_cast<CubeCollider*>(t_other), t_m);
            default:
                return {};
            }
        }

        glm::vec3 CubeCollider::getClosesPointToCube(glm::vec3 t_point)
        {
            glm::vec3 result;
            glm::vec3 center = getTransform()->getPos();

            for (int axis = 0; axis < 3; axis++)
            {
                if (t_point[axis] > center[axis] + m_max[axis])
                {
                    result[axis] = center[axis] + m_max[axis];
                } else if (t_point[axis] < center[axis] + m_min[axis]) {
                    result[axis] = center[axis] + m_min[axis];
                } else {
                    result[axis] = t_point[axis];
                }
            }

            return result;
        }

        void CubeCollider::setMeshVertices()
        {
            float w = m_size.x;
            float h = m_size.y;
            float d = m_size.z;

            m_colliderVertices = {
                { {-(w/2.0f), -(h/2.0f), -(d/2.0f) } }, // -- front face
                { { (w/2.0f), -(h/2.0f), -(d/2.0f) } },
                { { (w/2.0f),  (h/2.0f), -(d/2.0f) } },
                { {-(w/2.0f),  (h/2.0f), -(d/2.0f) } }, // -- front face
                
                { {-(w/2.0f), -(h/2.0f),  (d/2.0f) } }, // -- back face
                { { (w/2.0f), -(h/2.0f),  (d/2.0f) } },
                { { (w/2.0f),  (h/2.0f),  (d/2.0f) } },
                { {-(w/2.0f),  (h/2.0f),  (d/2.0f) } }, // -- back face

                { {-(w/2.0f),  (h/2.0f),  (d/2.0f) } }, // -- side right
                { {-(w/2.0f),  (h/2.0f), -(d/2.0f) } },
                { {-(w/2.0f), -(h/2.0f), -(d/2.0f) } },
                { {-(w/2.0f), -(h/2.0f),  (d/2.0f) } }, // -- side right
                
                { { (w/2.0f),  (h/2.0f),  (d/2.0f) } }, // -- side left
                { { (w/2.0f),  (h/2.0f), -(d/2.0f) } },
                { { (w/2.0f), -(h/2.0f), -(d/2.0f) } },
                { { (w/2.0f), -(h/2.0f),  (d/2.0f) } }, // -- side left

                { {-(w/2.0f), -(h/2.0f), -(d/2.0f) } }, // -- bottom
                { { (w/2.0f), -(h/2.0f), -(d/2.0f) } },
                { { (w/2.0f), -(h/2.0f),  (d/2.0f) } },
                { {-(w/2.0f), -(h/2.0f),  (d/2.0f) } }, // -- bottom
                
                { {-(w/2.0f),  (h/2.0f), -(d/2.0f) } }, // -- top
                { { (w/2.0f),  (h/2.0f), -(d/2.0f) } },
                { { (w/2.0f),  (h/2.0f),  (d/2.0f) } },
                { {-(w/2.0f),  (h/2.0f),  (d/2.0f) } } // -- top
            };

            m_colliderIndices.reserve(8*6);
            std::vector<unsigned int> baseIndices = { 0,1, 1,2, 2,3, 3,0 };
            for (int i = 0; i < 6; i++)
            {
                for (int x = 0; x < 8; x++)
                {
                    m_colliderIndices.push_back(baseIndices[x] + (i*4));
                }
            }
        }

    }
}