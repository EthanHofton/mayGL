#include <mayGL/component/component.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        void Component::imguiComponentInspector()
        {
            std::string type;
            switch (m_type)
            {
            case transform:
                type = "transform";
                break;
            case mesh:
                type = "mesh";
                break;
            case color:
                type = "color";
                break;
            case texture:
                type = "texture";
                break;
            case textureAtlas:
                type = "textureAtlas";
                break;
            case physicsBody:
                type = "physicsBody";
                break;
            case collider:
                type = "collider";
                break;
            case material:
                type = "material";
                break;
            case renderer:
                type = "renderer";
                break;
            case custom:
                type = "custom";
                break;
            default:
                break;
            }

            ImGui::LabelText("m_parent", "%s", getParent()->getEntityId().c_str());
            ImGui::Separator();

            ImGui::LabelText("m_type", "%s (%i)", type.c_str(), (int)m_type);
            ImGui::Separator();

            ImGui::LabelText("m_id", "%s", m_id.c_str());
            ImGui::Separator();
        }
    }
}