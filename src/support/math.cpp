#include <mayGL/support/math.hpp>

namespace mayGL
{
    namespace math
    {
        float degToRad(float t_deg)
        {
            return ((t_deg*c_PI) / 180.0f);
        }
        
        float radToDeg(float t_rad)
        {
            return (c_PI / (180.0f*t_rad));
        }
        
        glm::vec3 modelToWorld(glm::vec3 &t_modelSpace, glm::mat4 &t_modelMatrix)
        {
            glm::vec4 worldSpace = glm::vec4(t_modelSpace.x, t_modelSpace.y, t_modelSpace.z, 1.0f);
            worldSpace = t_modelMatrix * worldSpace;
            
            return {worldSpace.x, worldSpace.y, worldSpace.z};
        }
        
        float map(float value, float min1, float max1, float min2, float max2) 
        {
            return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
        }

        float pythag1f(float a, float b)
        {
            return sqrt((a*a) + (b*b));
        }

        glm::vec2 pythag2f(glm::vec2 a, glm::vec2 b)
        {
            glm::vec2 result;
            for (int i = 0; i < 2; i++)
            {
                result[i] = sqrt((a[i]*a[i]) + (b[i]*b[i]));
            }

            return result;
        }

        glm::vec3 pythag3f(glm::vec3 a, glm::vec3 b)
        {
            glm::vec3 result;
            for (int i = 0; i < 3; i++)
            {
                result[i] = sqrt((a[i]*a[i]) + (b[i]*b[i]));
            }

            return result;
        }

        glm::vec4 pythag4f(glm::vec4 a, glm::vec4 b)
        {
            glm::vec4 result;
            for (int i = 0; i < 4; i++)
            {
                result[i] = sqrt((a[i]*a[i]) + (b[i]*b[i]));
            }

            return result;
        }

        bool equal(float a, float b)
        {
            return std::abs(a-b) <= c_EPSILON;
        }
    }
}
