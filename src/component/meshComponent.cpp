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
                // use shader files or shader text
                ImGui::Text("Shader to use:");
                static int shaderFileOrString = -1;
                if (shaderFileOrString == -1)
                {
                    if (!hasShader())
                    {
                        // use no shader
                        shaderFileOrString = 2;
                    } else {
                        auto shader = m_shaderManager->getShader(m_shaderId);
                        if (shader->hasVertexShaderFile() || shader->hasGeometryShaderFile() || shader->hasFragmentShaderFile())
                        {
                            // use shader file
                            shaderFileOrString = 1;
                        } else {
                            // use shader string
                            shaderFileOrString = 0;
                        }
                    }
                }

                ImGui::RadioButton(("Shader String##" + uidPrefix).c_str(), &shaderFileOrString, 0);
                ImGui::RadioButton(("Shader File##" + uidPrefix).c_str(), &shaderFileOrString, 1);
                ImGui::RadioButton(("No Shader##" + uidPrefix).c_str(), &shaderFileOrString, 2); 
                ImGui::Separator();

                if (shaderFileOrString == 0)
                {
                    static bool vertexEditing = false;
                    static bool geometryEditing = false;
                    static bool fragmentEditing = false;
                    static bool vertexEdited = false;
                    static bool geometryEdited = false;
                    static bool fragmentEdited = false;
                    static std::string vertexShader;
                    static std::string geometryShader;
                    static std::string fragmentShader;

                    ImGui::Text("Edit the text of the shaders");
                    ImGui::Separator();

                    ImGui::Text("Vertex Shader:");
                    if (ImGui::Button("Edit Vertex Shader"))
                    {
                        vertexEditing = true;
                        if (hasShader())
                        {
                            vertexShader = m_shaderManager->getShader(m_shaderId)->getVertexShader();
                        } else {
                            vertexShader = "";
                        }
                    }

                    ImGui::Text("Geometry Shader:");
                    if (ImGui::Button("Edit Geometry Shader"))
                    {
                        geometryEditing = true;
                        if (hasShader())
                        {
                            geometryShader = m_shaderManager->getShader(m_shaderId)->getGeometryShader();
                        } else {
                            geometryShader = "";
                        }
                    }

                    ImGui::Text("Fragment Shader:");
                    if (ImGui::Button("Edit Fragment Shader"))
                    {
                        fragmentEditing = true;
                        if (hasShader())
                        {
                            fragmentShader = m_shaderManager->getShader(m_shaderId)->getFragmentShader();
                        } else {
                            fragmentShader = "";
                        }
                    }
                    ImGui::Separator();

                    if (vertexEditing)
                    {
                        imguiShaderEditor(vertexShader, vertexEditing, vertexEdited, 0);
                    }

                    if (geometryEditing)
                    {
                        imguiShaderEditor(geometryShader, geometryEditing, geometryEdited, 1);
                    }

                    if (fragmentEditing)
                    {
                        imguiShaderEditor(fragmentShader, fragmentEditing, fragmentEdited, 2);
                    }
                    
                    ImGui::Text("Update %s shaders", m_id.c_str());
                    std::string updateShadersName = "Update##" + m_id + getParent()->getEntityId();
                    if (ImGui::Button(updateShadersName.c_str()))
                    {
                        if (vertexEdited || geometryEdited || fragmentEdited)
                        {
                            if (!vertexEdited)
                            {
                                if (hasShader())
                                {
                                    vertexShader = m_shaderManager->getShader(m_shaderId)->getVertexShader();
                                } else {
                                    vertexShader = "";
                                }
                            }

                            if (!geometryEdited)
                            {
                                if (hasShader())
                                {
                                    geometryShader = m_shaderManager->getShader(m_shaderId)->getGeometryShader();
                                } else {
                                    geometryShader = "";
                                }
                            }

                            if (!fragmentEdited)
                            {
                                if (hasShader())
                                {
                                    fragmentShader = m_shaderManager->getShader(m_shaderId)->getFragmentShader();
                                } else {
                                    fragmentShader = "";
                                }
                            }

                            if (geometryShader == "")
                            {
                                loadShaderString(vertexShader, fragmentShader);
                            } else {
                                loadShaderString(vertexShader, geometryShader, fragmentShader);
                            }
                        }
                    }
                }

                if (shaderFileOrString == 1)
                {
                    ImGui::Text("Edit the filepath of the shaders");
                    ImGui::Separator();

                    static std::string vertexShaderFile = (hasShader()) ? m_shaderManager->getShader(m_shaderId)->getVertexShaderFile() : "";
                    static std::string geometryShaderFile = (hasShader()) ? m_shaderManager->getShader(m_shaderId)->getGeometryShaderFile() : "";
                    static std::string fragmentShaderFile = (hasShader()) ? m_shaderManager->getShader(m_shaderId)->getFragmentShaderFile() : "";
                    ImGui::Text("Vertex shader path");
                    ImGui::InputTextWithHint("vertex shader", "Vertex Shader", &vertexShaderFile);

                    ImGui::Text("Geometry shader path");
                    ImGui::InputTextWithHint("geometry shader", "Geometry Shader", &geometryShaderFile);

                    ImGui::Text("Fragment shader path");
                    ImGui::InputTextWithHint("fragment shader", "Fragment Shader", &fragmentShaderFile);

                    bool update = true;
                    if (hasShader())
                    {
                        auto shader = m_shaderManager->getShader(m_shaderId);
                        update = shader->getVertexShaderFile() != vertexShaderFile;
                        update = update || shader->getGeometryShaderFile() != geometryShaderFile;
                        update = update || shader->getFragmentShaderFile() != fragmentShaderFile;
                    }

                    ImGui::Separator();
                    ImGui::Text("Update shader filepaths");
                    if (ImGui::Button("Update") && update)
                    {
                        if (vertexShaderFile == "" && geometryShaderFile == "" && fragmentShaderFile == "") {
                            setShader("");
                        } else if (geometryShaderFile == "")
                        {
                            loadShader(vertexShaderFile, fragmentShaderFile);
                        } else {
                            loadShader(vertexShaderFile, geometryShaderFile, fragmentShaderFile);
                        }
                    }
                }

                if (shaderFileOrString == 2)
                {
                    setShader("");
                }
            }
            ImGui::Separator();
        }

        void Mesh::imguiShaderEditor(std::string& t_shaderText, bool &t_editing, bool &t_updated, int t_id)
        {
            std::string shaderType = "";
            if (t_id == 0)
            {
                shaderType = "Vertex";
            } else if (t_id == 1) {
                shaderType = "Geometry";
            } else if (t_id == 2) {
                shaderType = "Fragment";
            }

            std::string windowName = "Editing " + m_id + " " + shaderType + " Shader###" + std::to_string(t_id) + m_id + getParent()->getEntityId();
            ImGui::Begin(windowName.c_str(), &t_editing);
            
            std::string inputTextName = "##source" + std::to_string(t_id) + m_id + getParent()->getEntityId();
            float height = ImGui::GetWindowHeight() - (ImGui::GetTextLineHeight() * 4.25f);
            ImGui::InputTextMultiline(inputTextName.c_str(), &t_shaderText, ImVec2(ImGui::GetWindowWidth(), height));

            std::string buttonName = "Save Changes##button" + std::to_string(t_id) + m_id + getParent()->getEntityId();
            if (ImGui::Button(buttonName.c_str()))
            {
                t_updated = true;
            }
            
            ImGui::End();
        }
    }
}
