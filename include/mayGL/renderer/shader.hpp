#ifndef shader_hpp
#define shader_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../core/logger.hpp"

namespace may
{
    namespace renderer
    {
        class Shader
        {
        public:
            
            Shader();
            ~Shader();
            
            void createFromString(std::string t_vertex, std::string t_frag);
            void createFromString(std::string t_vertex, std::string t_geom, std::string t_frag);
            void createFromFiles(std::string t_vertFile, std::string t_fragFile);
            void createFromFiles(std::string t_vertFile, std::string t_geomFile, std::string t_fragFile);
            
            std::string readFile(std::string t_filepath);
            
            unsigned int getUniformLocation(std::string t_unifromId);
            void addUniform(std::string t_unifromId);
            
            void setUniform1f(std::string t_uniformId, float t_val);
            void setUniform2f(std::string t_uniformId, glm::vec2 t_val);
            void setUniform3f(std::string t_uniformId, glm::vec3 t_val);
            void setUniform4f(std::string t_uniformId, glm::vec4 t_val);
            
            void useShader();
            void clearShader();
            
        private:
            
            unsigned int m_shaderId;
            
            std::map<std::string, unsigned int> m_uniformMap;
            
            // -- uniform maps
            std::map<std::string, float> m_uniform1fMap;
            std::map<std::string, glm::vec2> m_uniform2fMap;
            std::map<std::string, glm::vec3> m_uniform3fMap;
            std::map<std::string, glm::vec4> m_uniform4fMap;
            // -- uniform maps
            
            void compileShader(std::string t_vertex, std::string t_frag);
            void compileShader(std::string t_vertex, std::string t_geometry, std::string t_frag);
            void addShader(unsigned int t_program, const char *t_shaderCode, GLenum t_shaderType);
        };
    }
}


#endif