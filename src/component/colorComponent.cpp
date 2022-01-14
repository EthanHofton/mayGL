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
            m_color.a = t_alpha;

            updateColor();
        }

        void ColorComponent::setRGB(glm::vec3 t_rgb)
        {
            m_color = glm::vec4(t_rgb, m_color.a);

            updateColor();
        }

        void ColorComponent::setColor(glm::vec4 t_color)
        {
            m_color = t_color;

            updateColor();
        }

        void ColorComponent::setColor(std::string t_hex, float t_alpha)
        {
            m_color = color::hexToRgba(t_hex, t_alpha);
        }

        void ColorComponent::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getMeshComponent(t_meshId));

            updateColor();
        }

        void ColorComponent::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }

            updateColor();
        }

        void ColorComponent::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getMeshComponents();

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
                m_meshes.erase(m_meshes.begin() + index);
            }

            updateColor();
        }

        void ColorComponent::removeAllMeshes()
        {
            m_meshes.clear();
        }

        void ColorComponent::updateColor()
        {
            CORE_DEBUG("color updated");

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
    }
}