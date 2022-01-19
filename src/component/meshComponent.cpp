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

            m_vertexSource = "";
            m_geometrySource = "";
            m_fragmentSource = "";

            m_vertexSourceFilepath = "";
            m_geometrySourceFilepath = "";
            m_fragmentSourceFilepath = "";

            m_vertexSourceOpen = false;
            m_vertexSourceUpdated = false;
            m_geometrySourceOpen = false;
            m_geometrySourceUpdated = false;
            m_fragmentSourceOpen = false;
            m_fragmentSourceUpdated = false;
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

        void Mesh::setShader(std::string t_shaderId) 
        { 
            m_shaderId = t_shaderId;
            if (m_shaderId != "")
            {
                m_vertexSource = getShader()->getVertexShader();
                m_geometrySource = getShader()->getGeometryShader();
                m_fragmentSource = getShader()->getFragmentShader();
            }
        }

        void Mesh::imguiUpdateSources()
        {
            m_vertexSource = getShader()->getVertexShader();
            m_geometrySource = getShader()->getGeometryShader();
            m_fragmentSource = getShader()->getFragmentShader();

            m_vertexSourceUpdated = true;
            m_geometrySourceUpdated = true;
            m_fragmentSourceUpdated = true;
        }
        
        void Mesh::loadShader(std::string t_vertexFile, std::string t_fragFile)
        {
            m_shaderId = m_shaderManager->loadFromFile(t_vertexFile, t_fragFile);

            imguiUpdateSources();
        }
        
        void Mesh::loadShader(std::string t_vertexFile, std::string t_geomFile, std::string t_fragFile)
        {
            m_shaderId = m_shaderManager->loadFromFile(t_vertexFile, t_geomFile, t_vertexFile);
            imguiUpdateSources();
        }
        
        void Mesh::loadShaderString(std::string t_vertex, std::string t_frag)
        {
            m_shaderId = m_shaderManager->loadFromText(t_vertex, t_frag);
            imguiUpdateSources();
        }
        
        void Mesh::loadShaderString(std::string t_vertex, std::string t_geom, std::string t_frag)
        {
            m_shaderId = m_shaderManager->loadFromText(t_vertex, t_geom, t_frag);
            imguiUpdateSources();
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
            
            return m_shaderManager->getShader(m_shaderId);
        }

        void Mesh::setTexture(std::string t_textureId)
        {
            if (m_texture != nullptr)
            {
                removeTexture();
            }

            m_texture = getParent()->getComponent<Texture, component::texture>(t_textureId);
            m_texture->addMesh(m_id);
            CORE_INFO("mesh with id '{}' set texture with id '{}'", m_id, t_textureId);
        }

        void Mesh::removeTexture() 
        {
            m_texture->removeMesh(m_id);
            m_texture = nullptr;
            CORE_INFO("mesh with id '{}' removed texture", m_id);
        }

        void Mesh::writeShader(std::string t_filepath, std::string t_source)
        {
            std::ofstream writeFile("shaders/" + t_filepath, std::ios::trunc);

            writeFile << t_source << std::endl;

            writeFile.close();

            return;
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
                ImGui::LabelText("m_verticesSize", "%i bytes", m_verticesSize);

                // indice size in bytes
                ImGui::LabelText("m_indeicesSize", "%i bytes", m_indeicesSize);

                // vertice count
                int verticeCount = m_verticesSize / m_vertexLayout->getVertexStride();
                ImGui::LabelText("vertice count", "%i vertices", verticeCount);

                // indice count
                ImGui::LabelText("indice count", "%i indices", (int)(m_indeicesSize / sizeof(unsigned int)));
            }
            ImGui::Separator();

            // shaders
            if (ImGui::TreeNodeEx(("Shader##" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // section for vertex shader
                imguiShaderProperties("Vertex", m_vertexSourceFilepath, m_vertexSource, m_vertexSourceOpen);

                // section for geometry shader
                imguiShaderProperties("Grometry", m_geometrySourceFilepath, m_geometrySource, m_geometrySourceOpen);

                // section for fragment shader
                imguiShaderProperties("Fragment", m_fragmentSourceFilepath, m_fragmentSource, m_fragmentSourceOpen);

                ImGui::Text("Update changes");
                if (ImGui::Button(("Update##" + uidPrefix).c_str()))
                {
                    bool updated = m_vertexSourceUpdated;
                    updated |= m_geometrySourceUpdated;
                    updated |= m_fragmentSourceUpdated;

                    if (updated)
                    {
                        if (m_vertexSource == "" && m_geometrySource == "" && m_fragmentSource == "") {
                            m_shaderId = "";
                        } else if (m_geometrySource != "") {
                            loadShaderString(m_vertexSource, m_geometrySource, m_fragmentSource);
                        } else {
                            loadShaderString(m_vertexSource, m_fragmentSource);
                        }
                    }
                }
            }

            // vertex source editor
            if (m_vertexSourceOpen)
            {
                imguiShaderEditor("Vertex", m_vertexSource, m_vertexSourceOpen, m_vertexSourceUpdated);
            }

            // geometry source editor
            if (m_geometrySourceOpen)
            {
                imguiShaderEditor("Geometry", m_geometrySource, m_geometrySourceOpen, m_geometrySourceUpdated);
            }

            // vertex source editor
            if (m_fragmentSourceOpen)
            {
                imguiShaderEditor("Fragment", m_fragmentSource, m_fragmentSourceOpen, m_fragmentSourceUpdated);
            }

            ImGui::Separator();

            // TODO: Shader
            // TODO: vertex layout

            // Bound Texturess
            if (ImGui::TreeNode(("Bind Textures##" + uidPrefix).c_str()))
            {
                auto allTextures = getParent()->getComponents<Texture, texture>();
                int selected = -1;
                if (m_texture != nullptr)
                {
                    for (int i = 0; i < allTextures.size(); i++)
                    {
                        if (allTextures[i]->getId() == m_texture->getId())
                        {
                            selected = i;
                        }
                    }
                }

                for (int i = 0; i < allTextures.size(); i++)
                {
                    if (ImGui::Selectable((allTextures[i]->getId() + "##" + uidPrefix).c_str(), selected == i))
                    {
                        if (selected == i)
                        {
                            selected = -1;
                            removeTexture();
                        } else {
                            selected = i;
                            setTexture(allTextures[i]->getId());
                        }
                    }
                }
                ImGui::TreePop();
            }

            ImGui::Separator();
        }

        void Mesh::imguiShaderProperties(std::string t_type, std::string &t_sourceFile, std::string &t_source, bool &t_open)
        {
            // uid suffix (oops)
            std::string uidPrefix = t_type + m_id + getParent()->getEntityId();

            if (ImGui::TreeNode((t_type + " shader##" + uidPrefix).c_str()))
            {
                // display label for loading source
                ImGui::Text("Load source file");

                // file input bar and button to edit source
                ImGui::InputTextWithHint(("Source filepath##" + uidPrefix).c_str(), "Filename...", &t_sourceFile);
                if (ImGui::Button(("Load Source##" + uidPrefix).c_str()))
                {
                    if (t_sourceFile != "")
                    {
                        t_source = renderer::Shader::readFile(t_sourceFile);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button(("Write Source##" + uidPrefix).c_str()))
                {
                    if (t_sourceFile != "")
                    {
                        // write source to input source file
                        writeShader(t_sourceFile, t_source);
                    }
                }

                ImGui::Separator();

                // display lable for editing source
                ImGui::Text("View/eidt source");

                if (ImGui::Button(("Edit Source##" + uidPrefix).c_str()))
                {
                    t_open = true;
                }

                ImGui::Separator();

                ImGui::TreePop();
            }
        }

        void Mesh::imguiShaderEditor(std::string t_type, std::string& t_shaderText, bool &t_editing, bool &t_updated)
        {
            // uid suffix (oops)
            std::string uidPrefix = t_type + m_id + getParent()->getEntityId();

            // begin window with uid name
            std::string windowName = "Editing " + m_id + " " + t_type + " Shader###" + uidPrefix;
            ImGui::Begin(windowName.c_str(), &t_editing);
            
            // input sourece
            ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            float height = ImGui::GetWindowHeight() - (ImGui::GetTextLineHeight() * 4.25f);
            ImGui::InputTextMultiline(("##source" + uidPrefix).c_str(), &t_shaderText, ImVec2(ImGui::GetWindowWidth(), height), flags);

            // commit changes button 
            if (ImGui::Button(("Save Changes##button" + uidPrefix).c_str()))
            {
                t_updated = true;
            }
            
            // end window
            ImGui::End();
        }
    }
}
