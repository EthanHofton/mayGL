#ifndef camera2D_hpp
#define camera2D_hpp

#include "camera.hpp"

namespace mayGL
{
    namespace renderer
    {
        class Camera2D : public Camera
        {
        public:
            
            Camera2D();
            ~Camera2D();
            
            void update() override;
            glm::mat4 getViewMatrix() override;
        };
    }
}


#endif