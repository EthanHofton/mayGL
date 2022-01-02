#include <geometry/rectangle.hpp>

namespace may
{
    namespace geometry
    {
        Rectangle::Rectangle(glm::vec2 t_size)
        {
            m_meshId = "rectangleMesh";
            m_meshTransformId = "rectangleMeshTransform";

            m_size = t_size;
            m_vertexData = {
                { {-m_size.x/2.0f,-m_size.y/2.0f, 0.0f}, {1,1,1,1}, {0,1}, 0 },
                { { m_size.x/2.0f,-m_size.y/2.0f, 0.0f}, {1,1,1,1}, {1,1}, 0 },
                { { m_size.x/2.0f, m_size.y/2.0f, 0.0f}, {1,1,1,1}, {1,0}, 0 },
                { {-m_size.x/2.0f, m_size.y/2.0f, 0.0f}, {1,1,1,1}, {0,0}, 0 },
            };
            
            m_indexData = {
                0,1,2,
                0,2,3
            };
            
            vertex::VertexLayout *vLayout = new vertex::VertexLayout;
            vLayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(rectangle_vertex, m_pos)));
            vLayout->push(new vertex::VertexComponent("color", vertex::color, 4, GL_FLOAT, GL_FALSE, offsetof(rectangle_vertex, m_color)));
            vLayout->push(new vertex::VertexComponent("tex_coord", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(rectangle_vertex, m_texCoord)));
            vLayout->push(new vertex::VertexComponent("tex_unit", vertex::texture_unit, 1, GL_FLOAT, GL_FALSE, offsetof(rectangle_vertex, m_texUnit)));
            
            addComponent(new component::Mesh(this, m_meshId, vLayout, GL_TRIANGLES));
            addComponent(new component::Transform(this, m_meshTransformId));
            
            getMeshComponent(m_meshId)->setVertices((void*)&m_vertexData[0], (int)(sizeof(rectangle_vertex)*m_vertexData.size()));
            getMeshComponent(m_meshId)->setIndices((void*)&m_indexData[0], sizeof(m_indexData));
            getMeshComponent(m_meshId)->loadShader("main.vertex", "main.fragment");
            getTransformComponent(m_meshTransformId)->addMesh(m_meshId);
        }

        component::Mesh *Rectangle::getRectMesh()
        {
            return getMeshComponent(m_meshId);
        }

        component::Transform *Rectangle::getRectMeshTransform()
        {
            return getTransformComponent(m_meshTransformId);
        }
    }
}
