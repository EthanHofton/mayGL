#include <projection/perspectiveProjection.hpp>

namespace may
{
    namespace renderer
    {
        PerspectiveProjection::PerspectiveProjection()
        {
            float width = GAME_CONFIG()["width"].GetInt();
            float height = GAME_CONFIG()["height"].GetInt();
            float z_near = GAME_CONFIG()["z_near_perspective"].GetFloat();
            float z_far = GAME_CONFIG()["z_far_perspective"].GetFloat();
            m_projectionMatrix = glm::perspective(45.0f, width/height, z_near, z_far);
        }
        
        PerspectiveProjection::~PerspectiveProjection() {}
    }
}
