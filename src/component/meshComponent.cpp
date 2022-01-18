#include <mayGL/component/meshComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        Mesh::Mesh(entity::Entity *t_parent, std::string t_id, vertex::VertexLayout *t_layout, unsigned int t_primativeType) : Component(t_parent, mesh, t_id)
        {
            m_indices = nullptr;
            m_worldVertices = nullptr;
            m_modelVertices = nullptr;
            m_primativeType = t_primativeType;
            m_shaderId = "";
            m_vertexLayout = t_layout;
            m_isVisable = true;
            m_texture = nullptr;
            
            m_shaderManager = renderer::ShaderManager::instance();
        }
        
        Mesh::~Mesh()
        {
            delete m_vertexLayout;
            m_vertexLayout = nullptr;
            
            if (m_modelVertices != nullptr)
            {
                free(m_modelVertices);
            }

            if (m_worldVertices != nullptr)
            {
                free(m_worldVertices);
            }

            m_modelVertices = nullptr;
            m_worldVertices = nullptr;
        }
        
        void Mesh::loadShader(std::string t_vertexFile, std::string t_fragFile)
        {
            m_shaderId = m_shaderManager->loadFromFile(t_vertexFile, t_fragFile);
        }
        
        void Mesh::loadShader(std::string t_vertexFile, std::string t_geomFile, std::string t_fragFile)
        {
            m_shaderId = m_shaderManager->loadFromFile(t_vertexFile, t_geomFile, t_vertexFile);
        }
        
        void Mesh::loadShaderString(std::string t_vertex, std::string t_frag)
        {
            m_shaderId = m_shaderManager->loadFromText(t_vertex, t_frag);
        }
        
        void Mesh::loadShaderString(std::string t_vertex, std::string t_geom, std::string t_frag)
        {
            m_shaderId = m_shaderManager->loadFromText(t_vertex, t_geom, t_frag);
        }
        
        void Mesh::setVertices(void *t_vertices, unsigned int t_vSize)
        {
            // set the size
            m_verticesSize = t_vSize;

            // delete the old opy of the vertices (world and modlel)
            if (m_modelVertices != nullptr)
            {
                free(m_modelVertices);
            }

            if (m_worldVertices != nullptr)
            {
                free(m_worldVertices);
            }

            // create new world and model vertice list
            m_worldVertices = malloc(m_verticesSize);
            m_modelVertices = malloc(m_verticesSize);

            // copy in the vertice data
            memcpy(m_modelVertices, t_vertices, m_verticesSize);
            memcpy(m_worldVertices, t_vertices, m_verticesSize);
            
            // update the texture coords
            if (getParent()->hasComponent(component::textureAtlas))
            {
                for (auto component : getParent()->getComponents<TextureAtlas, component::textureAtlas>())
                {
                    component->setTextureCoords();
                }
            }

            // update the color
            if (getParent()->hasComponent(component::color))
            {
                for (auto component : getParent()->getComponents<ColorComponent, component::color>())
                {
                    component->updateColor();
                }
            }
        }
        
        void Mesh::setIndices(void *t_indices, unsigned int t_iSize)
        {
            // set the indices size IN BYTES
            m_indeicesSize = t_iSize;

            // delete the old indice array allready if set
            if (m_indices != nullptr)
            {
                free(m_indices);
            }

            // create a new indice list
            m_indices = malloc(m_indeicesSize);

            // copy over the indcie data
            memcpy(m_indices, t_indices, m_indeicesSize);
        }
        
        renderer::Shader *Mesh::getShader()
        {
            if (m_shaderId == "")
            {
                return nullptr;
            }
            
            return renderer::ShaderManager::instance()->getShader(m_shaderId);
        }

        void Mesh::setTexture(std::string t_textureId)
        {
            m_texture = getParent()->getComponent<Texture, component::texture>(t_textureId);
        }
    }
}
