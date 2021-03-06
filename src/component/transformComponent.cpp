#include <mayGL/component/transformComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        Transform::Transform(entity::Entity *t_parent, std::string t_id) : Component(t_parent, transform, t_id)
        {
            m_scaleMatrix = glm::mat4(1.0f);
            m_translationMatrix = glm::mat4(1.0f);
            m_rotationMatrix = glm::mat4(1.0f);
            m_modelMatrix = glm::mat4(1.0f);
            
            m_pos = glm::vec3(0.0f);
            m_scale = glm::vec3(1.0f);
            m_roatation = 0.0f;
            
            m_needsMatrixUpdate = true;
            m_transformUpdate = false;

            m_rotAxis = glm::vec3(0,0,0);
            m_dragEdit = true;
        }
        
        Transform::~Transform() {}
        
        void Transform::rotate(float t_rad, glm::vec3 t_axis)
        {
            if (t_rad == 0)
            {
                return;
            }

            m_roatation += t_rad;
            if (m_roatation >= math::c_TAU)
            {
                m_roatation -= math::c_TAU;
            }
                
            if (m_roatation < 0)
            {
                m_roatation += math::c_TAU;
            }
                
            m_rotationMatrix = glm::rotate(m_rotationMatrix, t_rad, t_axis);
            m_needsMatrixUpdate = true;
        }
        
        void Transform::setRotation(float t_rad, glm::vec3 t_axis)
        {
            m_rotationMatrix = glm::rotate(glm::mat4(1.0f), t_rad, t_axis);
            m_needsMatrixUpdate = true;
            m_roatation = t_rad;
        }
        
        float Transform::getRoatation(space t_s)
        {
            if (t_s == model)
                return m_roatation;
            
            if (!getParent()->hasParent())
            {
                return m_roatation;
            } else {
                if (getParent()->getParent()->hasComponent(transform))
                {
                    Transform *t = static_cast<Transform*>(getParent()->getParent()->findComponentByType(transform)[0]);
                    return t->getRoatation();
                } else {
                    return m_roatation;
                }
            }
        }
        
        void Transform::scale(glm::vec3 t_scale)
        {
            m_scaleMatrix = glm::scale(m_scaleMatrix, t_scale);
            m_needsMatrixUpdate = true;
            m_scale = {m_scale.x*t_scale.x, m_scale.y*t_scale.y, m_scale.z*t_scale.z};
        }
        
        void Transform::setScale(glm::vec3 t_scale)
        {
            if (t_scale == m_scale)
            {
                return;
            }

            m_scaleMatrix = glm::scale(glm::mat4(1.0f), t_scale);
            m_needsMatrixUpdate = true;
            m_scale = t_scale;
        }
        
        glm::vec3 Transform::getScale(space t_s)
        {
            if (t_s == model)
                return m_scale;
            
            if (!getParent()->hasParent())
            {
                return m_scale;
            } else {
                if (getParent()->getParent()->hasComponent(transform))
                {
                    Transform *t = static_cast<Transform*>(getParent()->getParent()->findComponentByType(transform)[0]);
                    return {m_scale.x*t->getScale().x, m_scale.y*t->getScale().y, m_scale.z*t->getScale().z};
                } else {
                    return m_scale;
                }
            }
        }
        
        void Transform::pos(glm::vec3 t_pos)
        {
            if (m_pos == t_pos)
            {
                return;
            }

            m_pos = t_pos;
            m_translationMatrix = glm::translate(glm::mat4(1.0f), m_pos);
            m_needsMatrixUpdate = true;
        }
        
        void Transform::translate(glm::vec3 t_translation)
        {
            pos(getPos(model) + t_translation);
        }
        
        glm::vec3 Transform::getPos(space t_s)
        {
            if (t_s == model)
                return m_pos;
            
            if (!getParent()->hasParent())
            {
                return m_pos;
            } else {
                if (getParent()->getParent()->hasComponent(transform))
                {
                    Transform *t = static_cast<Transform*>(getParent()->getParent()->findComponentByType(transform)[0]);
                    return m_pos + t->getPos();
                } else {
                    return m_pos;
                }
            }
        }

        void Transform::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getComponent<Mesh, component::mesh>(t_meshId));
            m_needsMatrixUpdate = true;
            CORE_INFO("Transform with id '{}' added mesh with id '{}'", m_id, t_meshId);
        }

        void Transform::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }

            m_needsMatrixUpdate = true;
            CORE_INFO("Transform with id '{}' added {} meshes", m_id, t_meshIds.size());
        }

        void Transform::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getComponents<Mesh, component::mesh>();

            m_needsMatrixUpdate = true;
            CORE_INFO("Transform with id '{}' added all meshes", m_id);
        }

        void Transform::removeMesh(std::string t_meshId)
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
                CORE_INFO("Transform with id '{}' removed mesh with id '{}'", m_id, t_meshId);
                m_meshes.erase(m_meshes.begin() + index);
            }
        }

        void Transform::removeAllMeshes()
        {
            CORE_INFO("Transform with id '{}' removed all bound meshes", m_id);
            m_meshes.clear();
        }
        
        void Transform::update()
        {
            m_transformUpdate = false;
            
            if (m_needsMatrixUpdate)
            {
                if (getParent()->hasParent())
                {
                    if (getParent()->getParent()->hasComponent(transform))
                    {
                        Transform *t = static_cast<Transform *>( getParent()->getParent()->findComponentByType(transform)[0]);
                        m_modelMatrix = t->getModelMatrix() * (m_translationMatrix * m_rotationMatrix * m_scaleMatrix);
                    }
                } else {
                    m_modelMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
                }
                
                m_needsMatrixUpdate = false;
                m_transformUpdate = true;
                
                if (getParent()->hasChildren())
                {
                    for (auto child : getParent()->getChildren())
                    {
                        if (child->hasComponent(transform))
                        {
                            Transform *t = static_cast<Transform *>( child->findComponentByType(transform)[0]);
                            t->m_needsMatrixUpdate = true;
                        }
                    }
                }
                
                // update mesh vertices if the parent has a mesh
                for (auto mesh : m_meshes)
                {
                    auto vertexLayout = mesh->getLayout();
                    int positionOffset = -1;
                    int count = -1;
                    int step = -1;
                    for (auto component : vertexLayout->getComponents())
                    {
                        if (component->getType() == vertex::position)
                        {
                            positionOffset = component->getVertexOffset();
                            count = component->getCount();
                            step = component->getVertexComponentStride();
                        }
                    }
                    
                    if (positionOffset != -1)
                    {
                        auto worldVertices = mesh->getWorldVertices();
                        auto modelVertices = mesh->getModelVertices();
                        for (int i = 0; i < (mesh->getVerticesSize() / vertexLayout->getVertexStride()); i++)
                        {
                            glm::vec4 pos(0,0,0,1);
                            for (int x = 0; x < count; x++)
                            {
                                float *data = (float*)((char *)modelVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                                pos[x] = *data;
                            }
                            
                            glm::vec4 worldPos = getModelMatrix() * pos;
                            for (int x = 0; x < count; x++)
                            {
                                float *data = (float*)((char *)worldVertices + (positionOffset) + (i*vertexLayout->getVertexStride()) + (x*step));
                                *data = worldPos[x];
                            }
                        }
                    }
                }
            }
        }

        void Transform::imguiComponentInspector()
        {
            // Component Info
            Component::imguiComponentInspector();

            // should use drag input
            std::string uidSuffix = "##transform" + m_id + getParent()->getEntityId();
            ImGui::Checkbox(("Drag Edit Values" + uidSuffix).c_str(), &m_dragEdit);
            ImGui::Separator();

            // needs matrix update
            ImGui::LabelText("m_transformUpdate", (m_transformUpdate) ? "1" : "0");
            ImGui::Separator();

            // Pos
            glm::vec3 newPos = m_pos;
            ImGui::Text("m_pos");
            if (m_dragEdit)
            {
                ImGui::DragFloat3(("xyz pos" + uidSuffix).c_str(), &newPos[0], 0.1f);
            } else {
                ImGui::InputFloat3(("xyz pos" + uidSuffix).c_str(), &newPos[0]);
            }
            if (newPos != m_pos)
            {
                pos(newPos);
            }
            ImGui::Separator();

            // Scale
            glm::vec3 newScale = m_scale;
            ImGui::Text("m_scale");
            if (m_dragEdit)
            {
                ImGui::DragFloat3(("xyz scale" + uidSuffix).c_str(), &newScale[0], 0.1f);
            } else {
                ImGui::InputFloat3(("xyz scale" + uidSuffix).c_str(), &newScale[0]);
            }
            if (newScale != m_scale)
            {
                setScale(newScale);
            }
            ImGui::Separator();

            // Rotation
            float deg = m_roatation;
            ImGui::Text("m_rotation");
            bool update = false;

            if (m_dragEdit)
            {
                update = ImGui::SliderAngle(("deg" + uidSuffix).c_str(), &deg);
                bool axisUpdate = ImGui::SliderFloat3(("axis of rotation" + uidSuffix).c_str(), &m_rotAxis[0], -1.0f, 1.0f);
                update = update || axisUpdate;
            } else {
                update = ImGui::InputFloat(("deg" + uidSuffix).c_str(), &deg);
                bool axisUpdate = ImGui::InputFloat3(("axis of rotation" + uidSuffix).c_str(), &m_rotAxis[0]);
                update = update || axisUpdate;
                deg = math::degToRad(deg);
            }
            if (update && m_rotAxis != math::c_vec3Zero)
            {
                setRotation(deg, m_rotAxis);
            }            
            ImGui::Separator();

            // add bound meshes
            if (ImGui::TreeNode(("Bind Meshes" + uidSuffix).c_str()))
            {
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

                for (int i = 0; i < allMeshes.size(); i++)
                {
                    if (ImGui::Selectable((allMeshes[i]->getId() + uidSuffix).c_str(), (bool)meshSelections[i]))
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


            ImGui::Spacing();
        }
    }
}
