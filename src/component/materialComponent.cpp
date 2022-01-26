#include <mayGL/component/materialComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        Material::Material(entity::Entity *t_parent, std::string t_id) : Component(t_parent, material, t_id)
        {
            m_Ka = glm::vec3(1, 0, 0);
            m_Kd = glm::vec3(1, 0, 0);
            m_Ks = glm::vec3(1, 0, 0);
            m_Ns = 32.0f;

            m_mapKd = 0;
            m_mapKs = 0;
            m_bump = 0;

            m_dragEdit = true;
        }

        void Material::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getComponent<Mesh, component::mesh>(t_meshId));
            CORE_INFO("Material with id '{}' added mesh with id '{}'", m_id, t_meshId);
        }

        void Material::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }

            CORE_INFO("Material with id '{}' added {} meshes", m_id, t_meshIds.size());
        }

        void Material::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getComponents<Mesh, component::mesh>();

            CORE_INFO("Material with id '{}' added all meshes", m_id);
        }

        void Material::removeMesh(std::string t_meshId)
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
                CORE_INFO("Material with id '{}' removed mesh with id '{}'", m_id, t_meshId);
                m_meshes.erase(m_meshes.begin() + index);
            }
        }

        void Material::removeAllMeshes()
        {
            CORE_INFO("Material with id '{}' removed all bound meshes", m_id);
            m_meshes.clear();
        }

        void Material::setValue(component::Mesh *mesh, vertex::VertexComponentTypes t_type, float *t_data)
        {
            if (mesh->getLayout()->hasComponent(t_type))
            {
                auto vertexLayout = mesh->getLayout();
                auto worldVertices = mesh->getWorldVertices();
                std::vector<vertex::VertexComponent*> components;
                for (auto component : vertexLayout->getComponents())
                {
                    if (component->getType() == t_type)
                    {
                        components.push_back(component);
                    }
                }

                for (auto component : components)
                {
                    int step = component->getVertexComponentStride();
                    int positionOffset = component->getVertexOffset();
                    int count = component->getCount();
                    for (int i = 0; i < (mesh->getVerticesSize() / vertexLayout->getVertexStride()); i++)
                    {
                        for (int x = 0; x < count; x++)
                        {
                            float *data = (float*)((char *)worldVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                            *data = t_data[x];
                        }
                    }
                }
            }
        }

        void Material::update()
        {
            // update Ka, Kd, Ks, Ns, map_Kd, map_Ks, bump
            for (auto mesh : m_meshes)
            {
                // TODO: dirty flags to stop update every frame
                setValue(mesh, vertex::Ka, &m_Ka[0]);
                setValue(mesh, vertex::Kd, &m_Kd[0]);
                setValue(mesh, vertex::Ks, &m_Ks[0]);
                setValue(mesh, vertex::Ns, &m_Ns);
                
                setValue(mesh, vertex::map_Kd, &m_mapKd);
                setValue(mesh, vertex::map_Ks, &m_mapKs);
                setValue(mesh, vertex::bump, &m_bump);
            }
        }

        void Material::imguiComponentInspector()
        {
            // suffix
            std::string uidSuffix = "matieral" + m_id + getParent()->getEntityId();

            // Component Info
            Component::imguiComponentInspector();

            // darg edit
            ImGui::Checkbox(("Drag edit##dragEdit" + uidSuffix).c_str(), &m_dragEdit);

            // Show mat properties
            // ambient
            ImGui::Text("m_Ka (ambient)");
            if (m_dragEdit)
            {
                ImGui::DragFloat3(("rgb##ambient" + uidSuffix).c_str(), &m_Ka[0], 0.01f);
            } else {
                ImGui::InputFloat3(("rgb##ambient" + uidSuffix).c_str(), &m_Ka[0]);
            }
            
            // diffuse
            ImGui::Text("m_Kd (diffuse)");
            if (m_dragEdit)
            {
                ImGui::DragFloat3(("rgb##diffuse" + uidSuffix).c_str(), &m_Kd[0], 0.01f);
            } else {
                ImGui::InputFloat3(("rgb##diffuse" + uidSuffix).c_str(), &m_Kd[0]);
            }

            // specular
            ImGui::Text("m_Ks (specular)");
            if (m_dragEdit)
            {
                ImGui::DragFloat3(("rgb##specular" + uidSuffix).c_str(), &m_Ks[0], 0.01f);
            } else {
                ImGui::InputFloat3(("rgb##specular" + uidSuffix).c_str(), &m_Ks[0]);
            }
            
            // sininess
            ImGui::Text("m_Ns (shininess)");
            if (m_dragEdit)
            {
                ImGui::DragFloat(("Ns##shininess" + uidSuffix).c_str(), &m_Ns, 0.1f);
            } else {
                ImGui::InputFloat(("Ns##shininess" + uidSuffix).c_str(), &m_Ns);
            }
            //

            // map_Kd
            ImGui::Text("m_mapKd (diffuse map)");
            ImGui::InputFloat(("Ns##diffuseMap" + uidSuffix).c_str(), &m_mapKd, 1);

            // map_Ks
            ImGui::Text("m_mapKs (specular map)");
            ImGui::InputFloat(("Ns##specualrMap" + uidSuffix).c_str(), &m_mapKs, 1);

            // bump
            ImGui::Text("m_bump (normal map)");
            ImGui::InputFloat(("Ns##normalMap" + uidSuffix).c_str(), &m_bump, 1);

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

        }
    }
}