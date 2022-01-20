#include <mayGL/geometry/model.hpp>

namespace mayGL
{
    namespace geometry
    {
        Model::Model(std::string t_modelFilepath)
        {
            m_modelOBJFile = t_modelFilepath;
            m_meshID = "modelMesh";
            m_transformID = "modelTransform";

            vertex::VertexLayout *vLayout = new vertex::VertexLayout;
            vLayout->push(new vertex::VertexComponent("pos", vertex::position, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_pos)));
            vLayout->push(new vertex::VertexComponent("normal", vertex::normals, 3, GL_FLOAT, GL_FALSE, offsetof(modelVertex, m_normal)));
            
            m_modelTransform = new component::Transform(this, m_transformID);
            m_modelMesh = new component::Mesh(this, m_meshID, vLayout, GL_TRIANGLES);

            loadOBJ(t_modelFilepath, m_modelMesh);

            m_modelMesh->loadShader("light.vertex", "light.fragment");

            addComponent(m_modelMesh);
            addComponent(m_modelTransform);

            m_modelTransform->addMesh(m_meshID);
        }

        Model::~Model() {}

        void Model::loadOBJ(std::string t_path, component::Mesh *t_mesh)
        {
            std::vector<modelVertex> vertices;
            std::vector<unsigned int> indices;

            std::string modelFile = "models/" + t_path;
            tinyobj::attrib_t attributes;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> materials;
            std::string warning;
            std::string error;

            if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, modelFile.c_str()))
            {
                CORE_CRITICAL("tinyobj error: {} {}", warning, error);
            }

            std::unordered_map<modelVertex, unsigned int> uniqueVertices;
            for (const auto &shape : shapes)
            {
                for (const auto &index : shape.mesh.indices)
                {
                    glm::vec3 position{
                        attributes.vertices[3 * index.vertex_index + 0],
                        attributes.vertices[3 * index.vertex_index + 1],
                        attributes.vertices[3 * index.vertex_index + 2]};

                    glm::vec3 normals;              
                    if (index.normal_index >= 0)
                    {
                        normals = {
                            attributes.normals[3 * index.normal_index + 0],
                            attributes.normals[3 * index.normal_index + 1],
                            attributes.normals[3 * index.normal_index + 2]};
                    } else {
                        normals = {0,0,0};
                    }

                    modelVertex v = {position, normals};

                    if (uniqueVertices.count(v) == 0)
                    {
                        uniqueVertices[v] = static_cast<unsigned int>(vertices.size());
                        vertices.push_back(v);
                    }

                    indices.push_back(uniqueVertices[v]);
                }
            }

            t_mesh->setVertices(&vertices[0], sizeof(modelVertex)*vertices.size());
            t_mesh->setIndices(&indices[0], sizeof(unsigned int)*indices.size());
        }
    }
}