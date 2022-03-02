#ifndef math_hpp
#define math_hpp

#include <iostream>
#include <math.h>

#include <glm/glm.hpp>

namespace mayGL
{
    namespace math
    {
        const float c_PI = 3.14159265359f;
        const float c_HALF_PI = 1.57079632679f;
        const float c_TAU = 6.28318530718f;
        const float c_E = 2.71828182846f;
        const float c_EPSILON = 0.0001f;
        const glm::vec3 c_G = {0.0f, -9.81f, 0.0f};

        // comparison with c_EPSILON tolarance
        bool equal(float a, float b);

        float pythag1f(float a, float b);
        glm::vec2 pythag2f(glm::vec2 a, glm::vec2 b);
        glm::vec3 pythag3f(glm::vec3 a, glm::vec3 b);
        glm::vec4 pythag4f(glm::vec4 a, glm::vec4 b);
        
        float degToRad(float t_deg);
        float radToDeg(float t_rad);
        
        glm::vec3 modelToWorld(glm::vec3 &t_modelSpace, glm::mat4 &t_modelMatrix);
        float map(float value, float min1, float max1, float min2, float max2);
        
        const glm::vec3 c_up = {0.0f, -1.0f, 0.0f};
        const glm::vec3 c_down = {0.0f, 1.0f, 0.0f};
        const glm::vec3 c_left = {-1.0f, 0.0f, 0.0f};
        const glm::vec3 c_right = {1.0f, 0.0f, 0.0f};
        const glm::vec3 c_forward = {0.0f, 0.0f, 1.0f};
        const glm::vec3 c_back = {0.0f, 0.0f, -1.0f};
        const glm::vec3 c_vec3Zero = {0.0f, 0.0f, 0.0f};
        
        const glm::vec3 c_xAxis = {1.0f, 0.0f, 0.0f};
        const glm::vec3 c_yAxis = {0.0f, 1.0f, 0.0f};
        const glm::vec3 c_zAxis = {0.0f, 0.0f, 1.0f};

        const glm::vec3 c_xyAxis = {1.0f, 1.0f, 0.0f};
        const glm::vec3 c_xzAxis = {1.0f, 0.0f, 1.0f};
        const glm::vec3 c_zyAxis = {0.0f, 1.0f, 1.0f};
        const glm::vec3 c_xyzAxis = {1.0f, 1.0f, 1.0f};
        
        const glm::vec3 c_positiveX = { 1.0f,  0.0f,  0.0f};
        const glm::vec3 c_negativeX = {-1.0f,  0.0f,  0.0f};
        const glm::vec3 c_positiveY = { 0.0f,  1.0f,  0.0f};
        const glm::vec3 c_negativeY = { 0.0f, -1.0f,  0.0f};
        const glm::vec3 c_positiveZ = { 0.0f,  0.0f,  1.0f};
        const glm::vec3 c_negativeZ = { 0.0f,  0.0f, -1.0f};
    }
}

#endif