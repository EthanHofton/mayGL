#include <mayGL/physics/physicsBody.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace physics
    {
        PhysicsBody::PhysicsBody(entity::Entity *t_parent, std::string t_id) : component::Component(t_parent, component::physicsBody, t_id)
        {
            m_force = glm::vec3(0.0f);
            m_velocity = glm::vec3(0.0f);
            m_mass = 1.0f;
            m_invMass = 1.0f;
            m_gravityScale = 1.0f;
            m_restitution = 1.0f;
            m_staticFriction = 0.0f;
            m_dynamicFriction = 0.0f;

            m_imguiDf = glm::vec3(0);
            m_imguiDv = glm::vec3(0);

            m_dragEdit = true;
        }

        void PhysicsBody::addForce(glm::vec3 t_dforce)
        {
            m_force += t_dforce;
        }

        void PhysicsBody::setForce(glm::vec3 t_force)
        {
            m_force = t_force;
        }

        void PhysicsBody::setVelocity(glm::vec3 t_velocity)
        {
            m_velocity = t_velocity;
        }

        void PhysicsBody::addVelocity(glm::vec3 t_dvelocity)
        {
            m_velocity += t_dvelocity;
        }

        void PhysicsBody::setMass(float t_mass)
        {
            m_mass = t_mass;
            m_invMass = (m_mass == 0) ? 0 : (1.0f / m_mass);
        }

        void PhysicsBody::setGravityScale(float t_gravityScale)
        {
            m_gravityScale = t_gravityScale;
        }

        void PhysicsBody::addTransform(std::string t_transformId)
        {
            m_transforms.push_back(getParent()->getComponent<component::Transform, component::transform>(t_transformId));
        }

        void PhysicsBody::addTransforms(std::vector<std::string> t_transformIds)
        {
            for (auto transformId : t_transformIds)
            {
                addTransform(transformId);
            }
        }

        void PhysicsBody::addAllTransforms()
        {
            m_transforms.clear();
            m_transforms = getParent()->getComponents<component::Transform, component::transform>();
        }

        void PhysicsBody::removeTransform(std::string t_transformId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_transforms.size(); index++)
            {
                if (m_transforms[index]->getId() == t_transformId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                m_transforms.erase(m_transforms.begin() + index);
            }
        }

        void PhysicsBody::removeAllTransforms()
        {
            m_transforms.clear();
        }

        void PhysicsBody::imguiComponentInspector()
        {
            // Component Info
            Component::imguiComponentInspector();

            // uid prefix to not confuse other windows
            std::string uidPrefix = "##physics" + m_id + getParent()->getEntityId();

            ImGui::Checkbox(("Drag Edit Values" + uidPrefix).c_str(), &m_dragEdit);
            ImGui::Separator();

            if (ImGui::TreeNodeEx(("Properties" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // mass
                float mass = m_mass;
                if (m_dragEdit)
                {
                    ImGui::DragFloat(("m_mass" + uidPrefix).c_str(), &mass, 1.0f);
                } else {
                    ImGui::InputFloat(("m_mass" + uidPrefix).c_str(), &mass, 1.0f);
                }

                if (mass != m_mass)
                {
                    setMass(mass);
                }

                // inv mass label
                ImGui::LabelText("m_invMass", "%.2g", m_invMass);
                
                // gravity scale
                if (m_dragEdit)
                {
                    ImGui::DragFloat(("m_gravityScale" + uidPrefix).c_str(), &m_gravityScale, 0.1f);
                } else {
                    ImGui::InputFloat(("m_gravityScale" + uidPrefix).c_str(), &m_gravityScale);
                }

                // gravity scale
                if (m_dragEdit)
                {
                    ImGui::DragFloat(("m_restitution" + uidPrefix).c_str(), &m_restitution, 0.1f, 0.0f, 1.0f);
                } else {
                    ImGui::InputFloat(("m_restitution" + uidPrefix).c_str(), &m_restitution);
                }

                // stativ friction
                if (m_dragEdit)
                {
                    ImGui::DragFloat(("m_staticFriction" + uidPrefix).c_str(), &m_staticFriction, 0.1f, 0.0f, 1.0f);
                } else {
                    ImGui::InputFloat(("m_staticFriction" + uidPrefix).c_str(), &m_staticFriction);
                }

                // dynamic friction
                if (m_dragEdit)
                {
                    ImGui::DragFloat(("m_dynamicFriction" + uidPrefix).c_str(), &m_dynamicFriction, 0.1f, 0.0f, 1.0f);
                } else {
                    ImGui::InputFloat(("m_dynamicFriction" + uidPrefix).c_str(), &m_dynamicFriction);
                }
            }

            ImGui::Separator();

            if (ImGui::TreeNodeEx(("Velocity" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // show velocity
                ImGui::LabelText("m_velocity", "(%.2g, %.2g, %.2g)", m_velocity.x, m_velocity.y, m_velocity.z);

                // set velocity
                if (m_dragEdit)
                {
                    ImGui::DragFloat3(("Set velocity" + uidPrefix).c_str(), &m_velocity[0], 0.1f);
                } else {
                    ImGui::InputFloat3(("Set velocity" + uidPrefix).c_str(), &m_velocity[0]);
                }
                
                // add velocity
                if (m_dragEdit)
                {
                    ImGui::DragFloat3(("Vecloty to add" + uidPrefix).c_str(), &m_imguiDv[0], 0.1f);
                } else {
                    ImGui::InputFloat3(("Vecloty to add" + uidPrefix).c_str(), &m_imguiDv[0]);
                }
                if (ImGui::Button(("Add" + uidPrefix).c_str()))
                {
                    addVelocity(m_imguiDv);
                }
            }

            ImGui::Separator();

            if (ImGui::TreeNodeEx(("Force" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // show force
                ImGui::LabelText("m_force", "(%.2g, %.2g, %.2g)", m_force.x, m_force.y, m_force.z);

                // add force
                if (m_dragEdit)
                {
                    ImGui::DragFloat3(("Force" + uidPrefix).c_str(), &m_imguiDf[0], 0.1f);
                } else {
                    ImGui::InputFloat3(("Force" + uidPrefix).c_str(), &m_imguiDf[0]);
                }
                if (ImGui::Button(("Add" + uidPrefix).c_str()))
                {
                    addForce(m_imguiDf);
                }
            }

            ImGui::Separator();
            
            // bind transforms
            if (ImGui::TreeNode(("Bind Transforms" + uidPrefix).c_str()))
            {
                // add bound meshes
                auto allTransforms = getParent()->getComponents<component::Transform, component::transform>();
                std::vector<char> transformSelections;
                transformSelections.resize(allTransforms.size());
                std::fill(transformSelections.begin(), transformSelections.end(), false);
                for (int i = 0; i < allTransforms.size(); i++)
                {
                    for (auto transform : m_transforms)
                    {
                        if (transform->getId() == allTransforms[i]->getId())
                        {
                            transformSelections[i] = true;
                        }
                    }
                }

                for (int i = 0; i < allTransforms.size(); i++)
                {
                    if (ImGui::Selectable((allTransforms[i]->getId() + uidPrefix).c_str(), (bool)transformSelections[i]))
                    {
                        if ((bool)transformSelections[i] == true)
                        {
                            // remove mesh
                            removeTransform(allTransforms[i]->getId());
                        } else {
                            // add mesh
                            addTransform(allTransforms[i]->getId());
                        }
                    }
                }

                ImGui::TreePop();
            }

            ImGui::Separator();
        }
    }
}