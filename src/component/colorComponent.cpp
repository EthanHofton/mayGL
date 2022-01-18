#include <mayGL/component/colorComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        ColorComponent::ColorComponent(entity::Entity *t_parent, std::string t_id, glm::vec4 t_color, int t_start, int t_end) : Component(t_parent, component::color, t_id)
        {
            m_color = t_color;
            m_vertexStart = t_start;
            m_vertexEnd = t_end;

            updateColor();
        }

        ColorComponent::ColorComponent(entity::Entity *t_parent, std::string t_id, glm::vec3 t_color, int t_start, int t_end) : Component(t_parent, component::color, t_id)
        {
            m_color = glm::vec4(t_color, 1.0f);
            m_vertexStart = t_start;
            m_vertexEnd = t_end;

            updateColor();
        }
        ColorComponent::ColorComponent(entity::Entity *t_parent, std::string t_id, std::string t_hex, int t_start, int t_end) : Component(t_parent, component::color, t_id)
        {
            m_color = color::hexToRgba(t_hex);
            m_vertexStart = t_start;
            m_vertexEnd = t_end;

            updateColor();
        }

        void ColorComponent::setAlpha(float t_alpha)
        {
            if (m_color.a != t_alpha)
            {
                m_color.a = t_alpha;

                updateColor();
            }
        }

        void ColorComponent::setRGB(glm::vec3 t_rgb)
        {
            if (glm::vec3(m_color) != t_rgb)
            {
                m_color = glm::vec4(t_rgb, m_color.a);

                updateColor();
            }
        }

        void ColorComponent::setColor(glm::vec4 t_color)
        {
            if (m_color != t_color)
            {
                m_color = t_color;

                updateColor();
            }
        }

        void ColorComponent::setColor(std::string t_hex, float t_alpha)
        {
            if (m_color != color::hexToRgba(t_hex, t_alpha))
            {
                m_color = color::hexToRgba(t_hex, t_alpha);

                updateColor();
            }
        }

        void ColorComponent::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getComponent<Mesh, component::mesh>(t_meshId));
            CORE_INFO("ColorComponent with id '{}' added mesh with id '{}'", m_id, t_meshId);

            updateColor();
        }

        void ColorComponent::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }

            CORE_INFO("ColorComponent with id '{}' added {} meshes", m_id, t_meshIds.size());

            updateColor();
        }

        void ColorComponent::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getComponents<Mesh, component::mesh>();
            CORE_INFO("ColorComponent with id '{}' added all meshes", m_id);

            updateColor();
        }

        void ColorComponent::removeMesh(std::string t_meshId)
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
                CORE_INFO("ColorComponent with id '{}' removed mesh with id '{}'", m_id, t_meshId);
                m_meshes.erase(m_meshes.begin() + index);
            }

            updateColor();
        }

        void ColorComponent::removeAllMeshes()
        {
            m_meshes.clear();
            CORE_INFO("ColorComponent with id '{}' removed all bound meshes", m_id);
        }

        void ColorComponent::updateColor()
        {
            for (auto mesh : m_meshes)
            {
                auto vertexLayout = mesh->getLayout();
                int positionOffset = -1;
                int count = -1;
                int step = -1;
                for (auto component : vertexLayout->getComponents())
                {
                    if (component->getType() == vertex::color)
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

                        for (int x = 0; x < count; x++)
                        {
                            float *data = (float*)((char *)worldVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                            *data = m_color[x];
                        }
                    }
                }
            }
        }

        void ColorComponent::imguiComponentInspector()
        {
            // Component Info
            Component::imguiComponentInspector();

            // start vertices
            ImGui::LabelText("m_vertexStart", "%i", m_vertexStart);
            ImGui::Separator();

            // end vertices
            ImGui::LabelText("m_vertexEnd", "%i", m_vertexEnd);
            ImGui::Separator();

            // choose color
            ImGui::Text("m_color");
            glm::vec4 newColor = m_color;
            static bool alpha_preview = true;
            static bool alpha_half_preview = false;
            static bool drag_and_drop = true;
            static bool options_menu = true;
            static bool hdr = false;
            static bool hsv = false;
            ImGui::Checkbox("With Alpha Preview", &alpha_preview);
            ImGui::Checkbox("With Half Alpha Preview", &alpha_half_preview);
            ImGui::Checkbox("With Drag and Drop", &drag_and_drop);
            ImGui::Checkbox("With Options Menu", &options_menu);
            ImGui::Checkbox("With HDR", &hdr);
            ImGui::Checkbox("With HSV", &hsv);
            ImGuiColorEditFlags misc_flags = (hsv ? ImGuiColorEditFlags_DisplayHSV : 0) | (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);
            ImGui::ColorEdit4("rgba color", &newColor[0], misc_flags);
            if (newColor != m_color)
            {
                setColor(newColor);
            }
            ImGui::Separator();

            // add bound meshes
            auto allMeshes = getParent()->getComponents<Mesh, mesh>();
            std::vector<char> meshSelections;
            meshSelections.resize(allMeshes.size());
            std::fill(meshSelections.begin(), meshSelections.end(), false);
            for (int i = 0; i < allMeshes.size(); i++)
            {
                for (auto mesh : m_meshes)
                {
                    if (mesh->getId() == allMeshes[i]->getId())
                    {
                        meshSelections[i] = true;
                    }
                }
            }

            if (ImGui::TreeNode("Bind Meshes"))
            {
                for (int i = 0; i < allMeshes.size(); i++)
                {
                    if (ImGui::Selectable(allMeshes[i]->getId().c_str(), (bool)meshSelections[i]))
                    {
                        if ((bool)meshSelections[i] == true)
                        {
                            // remove mesh
                            removeMesh(allMeshes[i]->getId());
                        } else {
                            // add mesh
                            addMesh(allMeshes[i]->getId());
                        }
                    }
                }

                ImGui::TreePop();
            }

            ImGui::Separator();

            ImGui::Spacing();
        }
    }
}