#ifndef shaderManager_hpp
#define shaderManager_hpp

#include <iostream>
#include <map>

#include "shader.hpp"

namespace may
{
    namespace renderer
    {
        class ShaderManager
        {
        public:
            
            static ShaderManager* instance();
            static void release();
            
            std::string loadFromFile(std::string t_vertex, std::string t_fragment);
            std::string loadFromFile(std::string t_vertex, std::string t_geometry, std::string t_fragment);
            std::string loadFromText(std::string t_vertex, std::string t_fragment);
            std::string loadFromText(std::string t_vertex, std::string t_geometry, std::string t_fragment);
            
            Shader *getShader(std::string t_shaderId);
            std::string shaderID(Shader *t_s);
            
            void addUniform(std::string t_shaderId, std::string t_unifromId);
            unsigned int uniformLocation(std::string t_shaderId, std::string t_unifromId);
            
        private:
            
            ShaderManager();
            ~ShaderManager();
            
        private:
            
            std::map<std::string, Shader*>m_shaders;
            
            static ShaderManager* s_instance;
        };
    }
}

#endif