#include <physics/sphereCollider.hpp>
#include <physics/collisions.hpp>
#include <entity/entity.hpp>

namespace may
{
    namespace physics
    {
        SphereCollider::SphereCollider(entity::Entity *t_parent, std::string t_id, std::string t_physicsBodyId, std::string t_transformId, float t_radius) : Collider(t_parent, t_id, sphere, t_physicsBodyId, t_transformId)
        {
            m_radius = t_radius;

            setMeshVertices();

            getColliderMesh()->setVertices((void*)&m_colliderVertices[0], sizeof(colliderVertex)*m_colliderVertices.size());
            getColliderMesh()->setIndices((void*)&m_colliderIndices[0], sizeof(unsigned int)*m_colliderIndices.size());
        }

        bool SphereCollider::testCollision(Collider* t_other, Collision &t_m)
        {
            switch (t_other->getColliderType())
            {
            case sphere:
                return sphereVsSphere(this, static_cast<SphereCollider*>(t_other), t_m);
            case cube:
                return sphereVsCube(this, static_cast<CubeCollider*>(t_other), t_m);
            default:
                return {};
            }
        }

        glm::vec3 SphereCollider::closesPointToSphere(glm::vec3 t_point)
        {
            glm::vec3 pos = getTransform()->getPos();
            glm::vec3 gradient = t_point - pos;
            gradient = (gradient == math::c_vec3Zero) ? math::c_vec3Zero : glm::normalize(gradient);
            return pos + (m_radius * gradient);
        }

        void SphereCollider::setMeshVertices()
        {
            // set mesh points
            float sectorCount = 30.0f;
            float stackCount = 30.0f;
            float sectorStep = math::c_TAU / sectorCount;
            float stackStep = math::c_TAU / stackCount;
            
            for (int i = 0; i <= stackCount; i++)
            {
                float stackAngle = math::c_PI / 2 - i * stackStep;
                float xy = m_radius * cosf(stackAngle);
                float z = m_radius * sinf(stackAngle);
                
                for (int j = 0; j <= sectorCount; j++)
                {
                    float sectorAngle = j * sectorStep;
                    
                    float x = xy * cosf(sectorAngle);
                    float y = xy * sinf(sectorAngle);
                    
                    m_colliderVertices.push_back({ {x,y,z} });
                }
            }

            for (int i = 0; i < stackCount; i++)
            {
                int k1 = i * (sectorCount + 1);
                int k2 = k1 + sectorCount + 1;
                
                for (int j = 0; j < sectorCount; j++, k1++, k2++)
                {
                    if (i != 0)
                    {
                        m_colliderIndices.push_back(k1);
                        m_colliderIndices.push_back(k2);
                        m_colliderIndices.push_back(k1 + 1);
                    }
                    
                    if (i != (stackCount - 1))
                    {
                        m_colliderIndices.push_back(k1 + 1);
                        m_colliderIndices.push_back(k2);
                        m_colliderIndices.push_back(k2 + 1);
                    }
                }
            }
        }
        
    }
}