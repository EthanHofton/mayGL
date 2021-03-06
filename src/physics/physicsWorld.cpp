#include <mayGL/physics/physicsWorld.hpp>

namespace mayGL
{
    namespace physics
    {
        PhysicsWorld::PhysicsWorld()
        {
            m_timer = core::Timer::instance();
            m_t = 0.0f;
            m_gravity = math::c_G;
            m_gravityAffected = true;
        }

        void PhysicsWorld::removePhysicsEntity(std::string t_enitiyId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_physicsEntites.size(); index++)
            {
                if (m_physicsEntites[index]->getEntityId() == t_enitiyId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                CORE_INFO("PhysicsWorld with id '{}' removed entity with id '{}'", getEntityId(), t_enitiyId);
                m_physicsEntites.erase(m_physicsEntites.begin() + index);
            }
        }

        void PhysicsWorld::resolve(Collision &t_c)
        {
            // get physics bodies
            PhysicsBody *a = t_c.m_c1->getPhysicsBody();
            PhysicsBody *b = t_c.m_c2->getPhysicsBody();

            // calc relative velocities
            glm::vec3 rv = b->getVelocity() - a->getVelocity();

            // calc normal from contact points
            glm::vec3 n = t_c.m_e2Contact - t_c.m_e1Contact;
            float pd = glm::length(n);
            n = (n == math::c_vec3Zero) ? math::c_vec3Zero : glm::normalize(n);
            CORE_TRACE("collision occured - penitration: {}, n: ({}, {}, {})", pd, n.x, n.y, n.z);

            // calculate the relative velocity in terms of the normal direction
            float vecAlongNormal = glm::dot(rv, n);

            // do not resolve if moving away from eachother
            if (vecAlongNormal > 0)
            {
                return;
            }

            // get the minumum restitution (energy loss)
            float e = std::min(a->getRestitution(), b->getRestitution());

            // calculate impulce
            float j = -(1 + e) * vecAlongNormal;
            j /= (a->getInvMass() + b->getInvMass());

            // apply impulce
            glm::vec3 impulce = j * n;

            // calculate the sum of the masses in order to evenly distrubute the impulce
            a->addVelocity(-(a->getInvMass()) * impulce);
            b->addVelocity((b->getInvMass()) * impulce);

            // apply friction
            // recalculate relative velocity
            rv = b->getVelocity() - a->getVelocity();

            // solve for tangent
            glm::vec3 t = rv - (n * glm::dot(rv, n));
            for (int i = 0; i < 3; i++)
            {
                t[i] = (math::equal(t[i], 0)) ? 0 : t[i];
            }
            t = (t == math::c_vec3Zero) ? math::c_vec3Zero : glm::normalize(t);

            // solve for magnitude to apply along the friction vector
            float jt = -glm::dot(rv, t);
            jt /= (a->getInvMass() + b->getInvMass());

            if (math::equal(jt, 0))
            {
                return;
            }

            // use mu to approximate mu given fricition coefficents of each body
            float mu = math::pythag1f(a->getStaticFriction(), b->getStaticFriction());

            // clamp magnitude of friction and create impulce vector
            glm::vec3 frictionImpulce;
            if (abs(jt) < j * mu)
            {
                frictionImpulce = jt * t;
            } else {
                // use dynamic friction if static friction is too high
                float muDynamic = math::pythag1f(a->getDynamicFriction(), b->getDynamicFriction());
                frictionImpulce = -j * t * muDynamic;
            }

            // apply friction impulce
            a->addVelocity(-a->getInvMass() * frictionImpulce);
            b->addVelocity( b->getInvMass() * frictionImpulce);
        }

        void PhysicsWorld::intergrate(PhysicsBody *t_body)
        {
            for (auto transform : t_body->getTransforms())
            {
                glm::vec3 f = t_body->getForce();
                glm::vec3 g = t_body->getGravityScale() * m_gravity;

                // calculate accelaration
                glm::vec3 a = t_body->getInvMass() * f;

                // add acceliration due to gravity
                if (m_gravityAffected)
                {
                    a += g;
                }

                // add change in accelaration to velocity (dv = a*dt)
                t_body->addVelocity(a * (float)m_timer->getDeltaTime());

                // add change in velocity to position (dx = v*dt)
                transform->translate(t_body->getVelocity() * (float)m_timer->getDeltaTime());

                // clear force
                t_body->setForce(glm::vec3(0));
            }
        }

        void PhysicsWorld::update()
        {
            m_t += (float)m_timer->getDeltaTime();
            
            std::vector<Collision> collisions;
            for (auto e1 : m_physicsEntites)
            {
                if (e1->isActive())
                {
                    for (auto c1 : e1->getComponents<Collider, component::collider>())
                    {
                        for (auto e2 : m_physicsEntites)
                        {
                            if (e2->isActive() && e1 != e2)
                            {
                                for (auto c2 : e2->getComponents<Collider, component::collider>())
                                {
                                    Collision collisionManifold;
                                    if (c1->testCollision(c2, collisionManifold))
                                    {
                                        collisions.push_back(collisionManifold);
                                    }
                                }
                            }
                        }
                    }
                }

                for (auto collision : collisions)
                {
                    resolve(collision);
                }
            }

            for (auto e : m_physicsEntites)
            {
                // intergrate
                if (e->isActive())
                {
                    for (auto body : e->getComponents<PhysicsBody, component::physicsBody>())
                    {
                        intergrate(body);
                    }
                }

            }

            for (auto collision : collisions)
            {
                positionalCorrection(collision);
            }

            collisions.clear();
        }

        void PhysicsWorld::positionalCorrection(Collision &t_c)
        {
            glm::vec3 n = t_c.m_e2Contact - t_c.m_e1Contact;
            float penetration = glm::length(n);
            n = (n == math::c_vec3Zero) ? math::c_vec3Zero : glm::normalize(n);
            float a_invMass = t_c.m_c1->getPhysicsBody()->getInvMass();
            float b_invMass = t_c.m_c2->getPhysicsBody()->getInvMass();

            const float k_slop = 0.015f; // Penetration allowance
            const float percent = 1.0f; // Penetration percentage to correct
            glm::vec3 correction = (std::max( penetration - k_slop, 0.0f ) / (a_invMass + b_invMass)) * n * percent;

            t_c.m_c1->getTransform()->translate(-(correction * a_invMass));
            t_c.m_c2->getTransform()->translate( (correction * b_invMass));
        }

        void PhysicsWorld::customImguiProperties(std::vector<entity::Entity*> t_entites)
        {
            std::string uidSuffex = "##physics" + getEntityId() + std::to_string(getEntityUID());

            static bool dragEdit = true;
            ImGui::Checkbox(("Drag Edit Values" + uidSuffex).c_str(), &dragEdit);
            ImGui::Separator();

            // gravity controller
            if (dragEdit)
            {
                ImGui::DragFloat3(("m_gravity" + uidSuffex).c_str(), &m_gravity[0], 0.1f);
            } else {
                ImGui::InputFloat3(("m_gravity" + uidSuffex).c_str(), &m_gravity[0]);
            }
            ImGui::Separator();

            ImGui::Checkbox(("m_gravityAffected" + uidSuffex).c_str(), &m_gravityAffected);
            ImGui::Separator();

            // m_physicsEntites
            if (ImGui::TreeNode(("Physics Entites" + uidSuffex).c_str()))
            {
                // add bound meshes
                std::vector<char> entitySelections;
                entitySelections.resize(t_entites.size());
                std::fill(entitySelections.begin(), entitySelections.end(), false);
                for (int i = 0; i < t_entites.size(); i++)
                {
                    for (auto e : m_physicsEntites)
                    {
                        if (e->getEntityId() == t_entites[i]->getEntityId())
                        {
                            entitySelections[i] = true;
                        }
                    }
                }

                for (int i = 0; i < t_entites.size(); i++)
                {
                    if (ImGui::Selectable((t_entites[i]->getEntityId() + uidSuffex).c_str(), (bool)entitySelections[i]))
                    {
                        if ((bool)entitySelections[i] == true)
                        {
                            // remove enitity
                            removePhysicsEntity(t_entites[i]->getEntityId());
                        } else {
                            // add entity
                            addPhysicsEntity(t_entites[i]);
                        }
                    }
                }

                ImGui::TreePop();
            }

            ImGui::Separator();
        }
    }
}