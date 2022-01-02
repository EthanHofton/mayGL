#include <geometry/cube.hpp>

namespace may
{
    namespace geometry
    {
        Cube::Cube(glm::vec3 t_size)
        {
            m_cubeMeshId = "cubeMesh";
            m_cubeMeshTransformId = "cubeMeshTransform";

            glm::vec4 cubeFrontFaceColor =  {1,1,1,1};
            glm::vec4 cubeBackFaceColor =   {1,1,1,1};
            glm::vec4 cubeLeftFaceColor =   {1,1,1,1};
            glm::vec4 cubeRightFaceColor =  {1,1,1,1};
            glm::vec4 cubeTopFaceColor =    {1,1,1,1};
            glm::vec4 cubeBottomFaceColor = {1,1,1,1};
            
            float w = t_size.x;
            float h = t_size.y;
            float d = t_size.z;
            
            m_size = t_size;
            m_vertexData = {
                {{-(w/2.0f), -(h/2.0f), -(d/2.0f)}, cubeFrontFaceColor, {0.0f,1.0f}}, // -- front face
                {{ (w/2.0f), -(h/2.0f), -(d/2.0f)}, cubeFrontFaceColor, {1.0f,1.0f}},
                {{ (w/2.0f),  (h/2.0f), -(d/2.0f)}, cubeFrontFaceColor, {1.0f,0.0f}},
                {{-(w/2.0f),  (h/2.0f), -(d/2.0f)}, cubeFrontFaceColor, {0.0f,0.0f}}, // -- front face
                
                {{-(w/2.0f), -(h/2.0f),  (d/2.0f)}, cubeBackFaceColor, {0.0f,1.0f}}, // -- back face
                {{ (w/2.0f), -(h/2.0f),  (d/2.0f)}, cubeBackFaceColor, {1.0f,1.0f}},
                {{ (w/2.0f),  (h/2.0f),  (d/2.0f)}, cubeBackFaceColor, {1.0f,0.0f}},
                {{-(w/2.0f),  (h/2.0f),  (d/2.0f)}, cubeBackFaceColor, {0.0f,0.0f}}, // -- back face

                {{-(w/2.0f),  (h/2.0f),  (d/2.0f)}, cubeRightFaceColor, {0.0f,0.0f}}, // -- side right
                {{-(w/2.0f),  (h/2.0f), -(d/2.0f)}, cubeRightFaceColor, {1.0f,0.0f}},
                {{-(w/2.0f), -(h/2.0f), -(d/2.0f)}, cubeRightFaceColor, {1.0f,1.0f}},
                {{-(w/2.0f), -(h/2.0f),  (d/2.0f)}, cubeRightFaceColor, {0.0f,1.0f}}, // -- side right
                
                {{ (w/2.0f),  (h/2.0f),  (d/2.0f)}, cubeLeftFaceColor, {0.0f,0.0f}}, // -- side left
                {{ (w/2.0f),  (h/2.0f), -(d/2.0f)}, cubeLeftFaceColor, {1.0f,0.0f}},
                {{ (w/2.0f), -(h/2.0f), -(d/2.0f)}, cubeLeftFaceColor, {1.0f,1.0f}},
                {{ (w/2.0f), -(h/2.0f),  (d/2.0f)}, cubeLeftFaceColor, {0.0f,1.0f}}, // -- side left

                {{-(w/2.0f), -(h/2.0f), -(d/2.0f)}, cubeBottomFaceColor, {0.0f,1.0f}}, // -- bottom
                {{ (w/2.0f), -(h/2.0f), -(d/2.0f)}, cubeBottomFaceColor, {1.0f,1.0f}},
                {{ (w/2.0f), -(h/2.0f),  (d/2.0f)}, cubeBottomFaceColor, {1.0f,0.0f}},
                {{-(w/2.0f), -(h/2.0f),  (d/2.0f)}, cubeBottomFaceColor, {0.0f,0.0f}}, // -- bottom
                
                {{-(w/2.0f),  (h/2.0f), -(d/2.0f)}, cubeTopFaceColor, {0.0f,1.0f}}, // -- top
                {{ (w/2.0f),  (h/2.0f), -(d/2.0f)}, cubeTopFaceColor, {1.0f,1.0f}},
                {{ (w/2.0f),  (h/2.0f),  (d/2.0f)}, cubeTopFaceColor, {1.0f,0.0f}},
                {{-(w/2.0f),  (h/2.0f),  (d/2.0f)}, cubeTopFaceColor, {0.0f,0.0f}}, // -- top
            };
            
            m_indexData.reserve(6*6);
            std::vector<unsigned int> baseIndicesFront = { 0,1,2, 0,2,3 };
            std::vector<unsigned int> baseIndicesBack =  { 0,2,1, 0,3,2 };
            for (int i = 0; i < 6; i++)
            {
                for (int x = 0; x < 6; x++)
                {
                    if (i == 0 || i == 3 || i == 6)
                        m_indexData.push_back(baseIndicesFront[x] + (i*4));
                    else
                        m_indexData.push_back(baseIndicesBack[x] + (i*4));
                }
            }
            
            vertex::VertexLayout *vLayout = new vertex::VertexLayout;
            vLayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(cube_vertex, m_pos)));
            vLayout->push(new vertex::VertexComponent("color", vertex::color, 4, GL_FLOAT, GL_FALSE, offsetof(cube_vertex, m_color)));
            vLayout->push(new vertex::VertexComponent("tex_coord", vertex::texture_coords, 2, GL_FLOAT, GL_FALSE, offsetof(cube_vertex, m_texCoords)));
            vLayout->push(new vertex::VertexComponent("tex_unit", vertex::texture_unit, 1, GL_FLOAT, GL_FALSE, offsetof(cube_vertex, m_unit)));
            
            addComponent(new component::Mesh(this, m_cubeMeshId, vLayout, GL_TRIANGLES));
            addComponent(new component::Transform(this, m_cubeMeshTransformId));
            
            getCubeMesh()->setVertices((void*)&m_vertexData[0], (int)(sizeof(cube_vertex)*m_vertexData.size()));
            getCubeMesh()->setIndices((void*)&m_indexData[0], 6*6*sizeof(unsigned int));
            getCubeMesh()->loadShader("geomerty/cube/texturedCube.vertex", "geomerty/cube/cube.fragment");
            getCubeMeshTransform()->addMesh(m_cubeMeshId);
        }

        component::Mesh *Cube::getCubeMesh()
        {
            return getMeshComponent(m_cubeMeshId);
        }

        component::Transform *Cube::getCubeMeshTransform()
        {
            return getTransformComponent(m_cubeMeshTransformId);
        }
    }
}
