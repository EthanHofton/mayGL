#include <mayGL/geometry/sphere.hpp>

namespace may
{
    namespace geometry
    {
        Sphere::Sphere(float t_r)
        {
            m_meshId = "sphereMesh";
            m_meshTransformId = "sphereMeshTransform";
            m_radius = t_r;
            
            float sectorCount = 30.0f;
            float stackCount = 30.0f;
            float sectorStep = math::c_TAU / sectorCount;
            float stackStep = math::c_TAU / stackCount;
            float invRadius = 1.0f / m_radius;
            
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
                    
                    float nx = x * invRadius;
                    float ny = y * invRadius;
                    float nz = z * invRadius;
                    
                    float s = (float)j / sectorCount;
                    float t = (float)i / sectorCount;
                    
                    m_vertexData.push_back({ {x,y,z}, {1,1,1,1}, {nx, ny, nz}, {s,t} });
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
                        m_indexData.push_back(k1);
                        m_indexData.push_back(k2);
                        m_indexData.push_back(k1 + 1);
                    }
                    
                    if (i != (stackCount - 1))
                    {
                        m_indexData.push_back(k1 + 1);
                        m_indexData.push_back(k2);
                        m_indexData.push_back(k2 + 1);
                    }
                }
            }
            
            vertex::VertexLayout *vLayout = new vertex::VertexLayout;
            vLayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(sphere_vertex, m_pos)));
            vLayout->push(new vertex::VertexComponent("color", vertex::color, 4, GL_FLOAT, GL_FALSE, offsetof(sphere_vertex, m_color)));
            vLayout->push(new vertex::VertexComponent("normals", vertex::normals, 3, GL_FLOAT, GL_FALSE, offsetof(sphere_vertex, m_normals)));
            vLayout->push(new vertex::VertexComponent("texCoords", vertex::color, 2, GL_FLOAT, GL_FALSE, offsetof(sphere_vertex, m_textureCoords)));
            
            addComponent(new component::Mesh(this, m_meshId, vLayout, GL_TRIANGLES));
            addComponent(new component::Transform(this, m_meshTransformId));
            
            getMeshComponent(m_meshId)->setVertices((void*)&m_vertexData[0], (int)(sizeof(sphere_vertex)*m_vertexData.size()));
            getMeshComponent(m_meshId)->setIndices((void*)&m_indexData[0], (int)(sizeof(unsigned int)*m_indexData.size()));
            getMeshComponent(m_meshId)->loadShader("geomerty/sphere/sphere.vertex", "geomerty/sphere/sphere.fragment");
            getTransformComponent(m_meshTransformId)->addMesh(m_meshId);
        }

        component::Mesh *Sphere::getSphereMesh()
        {
            return getMeshComponent(m_meshId);
        }

        component::Transform *Sphere::getSphereMeshTransform()
        {
            return getTransformComponent(m_meshTransformId);
        }
    }
}
