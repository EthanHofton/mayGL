#include <mayGL/projection/orthographicProjection.hpp>

namespace mayGL
{
    namespace renderer
    {
        OrthographicProjection::OrthographicProjection()
        {
            float width = GAME_CONFIG()["width"].GetInt();
            float height = GAME_CONFIG()["height"].GetInt();
            float z_near = GAME_CONFIG()["z_near_ortho"].GetInt();
            float z_far = GAME_CONFIG()["z_far_ortho"].GetInt();
            m_projectionMatrix = glm::ortho(0.0f, width, height, 0.0f, z_near, z_far);
        }
        
        OrthographicProjection::~OrthographicProjection() {}
    }
}
