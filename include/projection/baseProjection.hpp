#ifndef baseProjection_hpp
#define baseProjection_hpp

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../core/config.hpp"

namespace may
{
    namespace renderer
    {
        class BaseProjection
        {
        public:
            
            inline BaseProjection() { m_projectionMatrix = glm::mat4(1.0f); }
            ~BaseProjection() = default;
            
            inline glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }
            
        protected:
            
            glm::mat4 m_projectionMatrix;
            
        };
    }
}


#endif