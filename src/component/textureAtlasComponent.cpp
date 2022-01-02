#include <component/textureAtlasComponent.hpp>
#include <entity/entity.hpp>

namespace may
{
    namespace component
    {
        TextureAtlas::TextureAtlas(entity::Entity *t_parent, std::string t_id, std::string t_jsonPath, std::string t_atlasId, int t_vstart, int t_vend) : Component(t_parent, textureAtlas, t_id)
        {
            m_type = textureAtlas;
            m_atlasId = t_atlasId;
            m_jsonPath = t_jsonPath;

            m_jsonConfig.Parse(getFile(m_jsonPath).c_str());

            glm::vec2 size = {m_jsonConfig["meta"]["width"].GetInt(), m_jsonConfig["meta"]["height"].GetInt()};
            m_coordMat = {
                1.0f / (float)size.x, 0.0f,
                0.0f,                 1.0f / (float)size.y
            };

            m_vertexStart = t_vstart;
            m_vertexEnd = t_vend;

            setTextureCoords();
        }

        void TextureAtlas::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getMeshComponent(t_meshId));
        }

        void TextureAtlas::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }
        }

        void TextureAtlas::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getMeshComponents();
        }

        void TextureAtlas::removeMesh(std::string t_meshId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_meshes.size(); index++)
            {
                if (m_meshes[index]->getId() == t_meshId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                m_meshes.erase(m_meshes.begin() + index);
            }
        }

        void TextureAtlas::removeAllMeshes()
        {
            m_meshes.clear();
        }

        void TextureAtlas::setTextureCoords()
        {
            for (auto mesh : m_meshes)
            {
                auto vertexLayout = mesh->getLayout();
                int positionOffset = -1;
                int count = -1;
                int step = -1;
                for (auto component : vertexLayout->getComponents())
                {
                    if (component->getType() == vertex::texture_coords)
                    {
                        positionOffset = component->getVertexOffset();
                        count = component->getCount();
                        step = component->getVertexComponentStride();
                    }
                }
                
                if (positionOffset != -1)
                {
                    auto worldVertices = mesh->getWorldVertices();
                    for (int i = 0; i < (mesh->getVerticesSize() / vertexLayout->getVertexStride()); i++)
                    {
                        if (m_vertexStart != -1)
                        {
                            if (i < m_vertexStart)
                            {
                                continue;
                            }
                        }

                        if (m_vertexEnd != -1)
                        {
                            if (i >= m_vertexEnd)
                            {
                                continue;
                            }
                        }

                        glm::vec2 coords;
                        for (int x = 0; x < count; x++)
                        {
                            float *data = (float*)((char *)worldVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                            coords[x] = *data;
                        }
                        // get size and offset for id
                        glm::vec2 size = {m_jsonConfig["data"][m_atlasId.c_str()]["width"].GetInt(), m_jsonConfig["data"][m_atlasId.c_str()]["height"].GetInt()};
                        glm::vec2 offset = {m_jsonConfig["data"][m_atlasId.c_str()]["x_offset"].GetInt(), m_jsonConfig["data"][m_atlasId.c_str()]["y_offset"].GetInt()};
                        coords.x *= size.x;
                        coords.y *= size.y;

                        coords.x += offset.x;
                        coords.y += offset.y;

                        coords = m_coordMat * coords;
                        
                        for (int x = 0; x < count; x++)
                        {
                            float *data = (float*)((char *)worldVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                            *data = coords[x];
                        }
                    }
                }
            }
        }

        std::string TextureAtlas::getFile(std::string t_filename)
        {
            std::string line;
            std::string output;
            std::ifstream file(t_filename);
            
            assert(file.is_open());
            
            while (std::getline(file, line))
            {
                output.append(line);
            }
            
            return output;
        }
    }
}