#ifndef color_hpp
#define color_hpp

#include <iostream>
#include <vector>
#include <sstream>

#include <glm/glm.hpp>

namespace may
{
    namespace color
    {
        glm::vec3 hexToRgb(std::string t_hex);
        glm::vec4 hexToRgba(std::string t_hex);
        glm::vec4 hexToRgba(std::string t_hex, float t_alpha);
        std::string rgbToHex(glm::vec3 rgb);
        std::string rgbToHex(glm::vec4 rgba);
    }
}

#endif