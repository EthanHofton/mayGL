#include <mayGL/camera/camera2D.hpp>

namespace mayGL
{
    namespace renderer
    {
        Camera2D::Camera2D() {}
        
        Camera2D::~Camera2D() {}
        
        void Camera2D::update() {}
        
        glm::mat4 Camera2D::getViewMatrix()
        {
            return getTransform()->getModelMatrix();
        }
    }
}
