#ifndef shader_hpp
#define shader_hpp

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "../core/logger.hpp"

namespace mayGL
{
    namespace renderer
    {
        class Shader
        {
        public:
            
            Shader();
            ~Shader();

            unsigned int getShaderId();
            
            void createFromString(std::string t_vertex, std::string t_frag);
            void createFromString(std::string t_vertex, std::string t_geom, std::string t_frag);
            void createFromFiles(std::string t_vertFile, std::string t_fragFile);
            void createFromFiles(std::string t_vertFile, std::string t_geomFile, std::string t_fragFile);
            
            static std::string readFile(std::string t_filepath);
            
            unsigned int getUniformLocation(std::string t_unifromId);
            void addUniform(std::string t_unifromId);
            
            void setUniform1f(std::string t_uniformId, float t_val);
            void setUniform2f(std::string t_uniformId, glm::vec2 t_val);
            void setUniform3f(std::string t_uniformId, glm::vec3 t_val);
            void setUniform4f(std::string t_uniformId, glm::vec4 t_val);
            
            void useShader();
            void clearShader();

            inline bool hasVertexShaderFile() { return !(m_vertexShaderFile == ""); }
            inline bool hasGeometryShaderFile() { return !(m_geometryShaderFile == ""); }
            inline bool hasFragmentShaderFile() { return !(m_fragmentShaderFile == ""); }

            inline bool hasVertexShader() { return !(m_vertexShader == ""); }
            inline bool hasGeometryShader() { return !(m_geometryShader == ""); }
            inline bool hasFragmentShader() { return !(m_fragmentShader == ""); }

            inline std::string getVertexShaderFile() { return m_vertexShaderFile; }
            inline std::string getGeometryShaderFile() { return m_geometryShaderFile; }
            inline std::string getFragmentShaderFile() { return m_fragmentShaderFile; }

            inline std::string getVertexShader() { return m_vertexShader; }
            inline std::string getGeometryShader() { return m_geometryShader; }
            inline std::string getFragmentShader() { return m_fragmentShader; }
            
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

            std::string m_vertexShaderFile;
            std::string m_geometryShaderFile;
            std::string m_fragmentShaderFile;

            std::string m_vertexShader;
            std::string m_geometryShader;
            std::string m_fragmentShader;
        };
    }
}


#endif