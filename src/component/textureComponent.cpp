#include <mayGL/component/textureComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        std::map<std::string, std::pair<int, int>> Texture::s_textureMap;
        
        Texture::Texture(entity::Entity *t_parent, std::string t_id, std::string t_filename) : Component(t_parent, texture, t_id)
        {
            m_filename = t_filename;
            std::string filepath = m_filename;
            m_updateTextureUnit = false;
            m_textureUnit = 0;
            
            int width, height;
            
            stbi_set_flip_vertically_on_load(false);
            unsigned char *texData = stbi_load(filepath.c_str(), &width, &height, &m_bitDepth, 0);
            m_size.x = width;
            m_size.y = height;
            
            if (s_textureMap.find(m_filename) == s_textureMap.end())
            {
                if (!texData)
                {
                    CORE_ERROR("error finding filename: {}", filepath);
                    assert(texData);
                }
                
                glGenTextures(1, &m_textureId);
                glBindTexture(GL_TEXTURE_2D, m_textureId);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
                glGenerateMipmap(GL_TEXTURE_2D);
                
                stbi_image_free(texData);
                
                s_textureMap[m_filename] = {m_textureId, 1};
            } else {
                m_textureId = s_textureMap[m_filename].first;
                s_textureMap[m_filename].second++;
                
                stbi_image_free(texData);
            }
        }
        
        Texture::~Texture()
        {
            if (s_textureMap[m_filename].second != 1)
                s_textureMap[m_filename].second--;
            else {
                s_textureMap.erase(s_textureMap.find(m_filename));
                glDeleteTextures(1, &m_textureId);
            }
        }
        
        void Texture::use()
        {
            glActiveTexture(GL_TEXTURE0 + m_textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }
        
        void Texture::setTextureUnit(unsigned int t_unit)
        {
            if (t_unit != m_textureUnit)
            {
                m_updateTextureUnit = true;
                m_textureUnit = t_unit;
            }
        }

        void Texture::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getMeshComponent(t_meshId));
        }

        void Texture::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }
        }

        void Texture::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getMeshComponents();
        }

        void Texture::removeMesh(std::string t_meshId)
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

        void Texture::removeAllMeshes()
        {
            m_meshes.clear();
        }
        
        void Texture::updateTextureUnit()
        {
            if (m_updateTextureUnit)
            {
                for (auto mesh : m_meshes)
                {
                    auto vertexLayout = mesh->getLayout();
                    int positionOffset = -1;
                    int count = -1;
                    int step = -1;
                    for (auto component : vertexLayout->getComponents())
                    {
                        if (component->getType() == vertex::texture_unit)
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
                            for (int x = 0; x < count; x++)
                            {
                                float *data = (float*)((char *)worldVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                                *data = m_textureUnit;
                            }
                        }
                    }
                }
                
                m_updateTextureUnit = false;
            }
        }
    }
}
