#include <mayGL/component/meshComponent.hpp>
#include <mayGL/entity/entity.hpp>


namespace mayGL
{
    namespace component
    {
        Mesh::Mesh(entity::Entity *t_parent, std::string t_id, vertex::VertexLayout *t_layout) : Component(t_parent, mesh, t_id)
        {
            m_indices = nullptr;
            m_worldVertices = nullptr;
            m_modelVertices = nullptr;
            m_vertexLayout = t_layout;
            m_isVisable = true;
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

        void Mesh::addTexture(std::string t_texId)
        {
            m_textures.push_back(getParent()->getComponent<Texture, component::texture>(t_texId));
            CORE_INFO("Mesh with id '{}' added texture with id '{}'", m_id, t_texId);
        }

        void Mesh::addTextures(std::vector<std::string> t_texIds)
        {
            for (auto tex : t_texIds)
            {
                addTexture(tex);
            }

            CORE_INFO("Mesh with id '{}' added {} textures", m_id, t_texIds.size());
        }

        void Mesh::addAllTextures()
        {
            m_textures.clear();
            m_textures = getParent()->getComponents<Texture, component::texture>();

            CORE_INFO("Mesh with id '{}' added all textures", m_id);
        }

        void Mesh::removeTexture(std::string t_texId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_textures.size(); index++)
            {
                if (m_textures[index]->getId() == t_texId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                CORE_INFO("Mesh with id '{}' removed texture with id '{}'", m_id, t_texId);
                m_textures.erase(m_textures.begin() + index);
            }
        }

        void Mesh::removeAllTextures()
        {
            CORE_INFO("Mesh with id '{}' removed all bound textures", m_id);
            m_textures.clear();
        }

        void Mesh::imguiComponentInspector()
        {
            // Component Info
            Component::imguiComponentInspector();

            // mesh visable
            std::string uidPrefix = m_id + getParent()->getEntityId();
            ImGui::Checkbox(("m_isVisable##" + uidPrefix).c_str(), &m_isVisable);
            ImGui::Separator();

            // vertice pointer and indice pointers
            if (ImGui::TreeNodeEx(("Data##" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // Display world Vetrcies pointers
                std::stringstream worldVerticesPointerStringstream;
                worldVerticesPointerStringstream << m_worldVertices;
                std::string worldVerticesPointerString = worldVerticesPointerStringstream.str();
                ImGui::LabelText("m_worldVertices", "%s", worldVerticesPointerString.c_str());

                // Display model Vetrcies pointers
                std::stringstream modelVerticesPointerStringstream;
                modelVerticesPointerStringstream << m_modelVertices;
                std::string modelVerticesPointerString = modelVerticesPointerStringstream.str();
                ImGui::LabelText("m_modelVertices", "%s", modelVerticesPointerString.c_str());

                // Display model Indices pointers
                std::stringstream indicesPointerStringstream;
                indicesPointerStringstream << m_indices;
                std::string indicesPointerString = indicesPointerStringstream.str();
                ImGui::LabelText("m_indices", "%s", indicesPointerString.c_str());

                // vertice size in bytes
                ImGui::LabelText("m_verticesSize", "%i bytes (%.2g Mb)", m_verticesSize, ((float)m_verticesSize / (1024.0f*1024.0f)));

                // indice size in bytes
                ImGui::LabelText("m_indeicesSize", "%i bytes (%.2g Mb)", m_indeicesSize, ((float)m_indeicesSize / (1024.0f*1024.0f)));

                // vertice count
                int verticeCount = m_verticesSize / m_vertexLayout->getVertexStride();
                int indiceCount = (int)(m_indeicesSize / sizeof(unsigned int));
                ImGui::LabelText("vertice count", "%i vertices", verticeCount);

                // indice count
                ImGui::LabelText("indice count", "%i indices", indiceCount);

                // triangle count
                ImGui::LabelText("triangle count", "%i triangles", (int)(indiceCount / 3));
            }
            ImGui::Separator();

            // TODO: vertex layout

            // Bound Texturess
            if (ImGui::TreeNode(("Bind Textures##" + uidPrefix).c_str()))
            {
                auto allTextures = getParent()->getComponents<Texture, texture>();
                std::vector<char> texSelections;
                texSelections.resize(allTextures.size());
                std::fill(texSelections.begin(), texSelections.end(), false);
                for (int i = 0; i < allTextures.size(); i++)
                {
                    for (auto tex : m_textures)
                    {
                        if (tex->getId() == allTextures[i]->getId())
                        {
                            texSelections[i] = true;
                        }
                    }
                }

                for (int i = 0; i < allTextures.size(); i++)
                {
                    if (ImGui::Selectable((allTextures[i]->getId() + "##" + uidPrefix).c_str(), (bool)texSelections[i]))
                    {
                        if ((bool)texSelections[i] == true)
                        {
                            // remove mesh
                            removeTexture(allTextures[i]->getId());
                        } else {
                            // add mesh
                            addTexture(allTextures[i]->getId());
                        }
                    }
                }
                ImGui::TreePop();
            }

            ImGui::Separator();
        }
    }
}
