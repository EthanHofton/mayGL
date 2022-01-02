#include <mayGL/camera/camera.hpp>

namespace may
{
    namespace renderer
    {
        Camera::Camera()
        {
            m_transformId = "cameraTransform";
            m_transform = new component::Transform(this, m_transformId);
            addComponent(m_transform);
        }
        
        Camera::~Camera() {}
    }
}
