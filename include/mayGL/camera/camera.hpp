#ifndef camera_hpp
#define camera_hpp

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../entity/entity.hpp"
#include "../component/transformComponent.hpp"

namespace may
{
    namespace renderer
    {
        class Camera : public entity::Entity
        {
        public:
            
            Camera();
            ~Camera();
            
            inline component::Transform *getTransform() { return m_transform; }
            inline std::string getCameraTransformId() { return m_transformId; }
            
            virtual glm::mat4 getViewMatrix() = 0;
            inline virtual void update() override {}
            
        private:
            
            component::Transform *m_transform;

            std::string m_transformId;
            
        };
    }
}


#endif