#include <mayGL/support/color.hpp>

namespace mayGL
{
    namespace color
    {
        glm::vec3 hexToRgb(std::string t_hex)
        {
            glm::vec3 rgb;
            
            for (int i = 0; i < 6; i += 2)
            {
                std::string pair;
                pair.push_back(t_hex[i + 0]);
                pair.push_back(t_hex[i + 1]);
                
                rgb[i/2] = (std::stoi(pair, nullptr, 16) / 0xFF);
            }
            
            return rgb;
        }
        
        glm::vec4 hexToRgba(std::string t_hex)
        {
            return hexToRgba(t_hex, 1.0f);
        }
        
        glm::vec4 hexToRgba(std::string t_hex, float t_alpha)
        {
            glm::vec3 rgb = hexToRgb(t_hex);
            
            return {rgb.r, rgb.g, rgb.b, t_alpha};
        }
        
        std::string rgbToHex(glm::vec3 rgb)
        {
            std::string hex;
            
            for (int i = 0; i < 3; i++)
            {
                int integerValue = rgb[i] * 0xFF;
                std::stringstream sstream;
                sstream << std::hex << integerValue;
                std::string result = sstream.str();
                
                if (integerValue < 10)
                {
                    std::string prev = result;
                    result = "0" + prev;
                }
                
                hex.append(result);
            }
            
            return hex;
        }
        
        std::string rgbToHex(glm::vec4 rgba)
        {
            return rgbToHex(glm::vec3(rgba.r, rgba.g, rgba.b));
        }
    }
}