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
            m_worldVertices = t_vertices;
            m_verticesSize = t_vSize;
            if (m_modelVertices != nullptr)
            {
                free(m_modelVertices);
            }
            m_modelVertices = malloc(m_verticesSize);
            memcpy(m_modelVertices, m_worldVertices, m_verticesSize);
            
            if (getParent()->hasComponent(component::textureAtlas))
            {
                for (auto component : getParent()->getComponents<TextureAtlas, component::textureAtlas>())
                {
                    component->setTextureCoords();
                }
            }

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
            m_indices = t_indices;
            m_indeicesSize = t_iSize;
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
