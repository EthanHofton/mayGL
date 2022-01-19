#include <mayGL/renderer/shader.hpp>

namespace mayGL
{
    namespace renderer
    {
        Shader::Shader()
        {
            m_shaderId = 0;
            m_vertexShader = "";
            m_geometryShader = "";
            m_fragmentShader = "";

            m_vertexShaderFile = "";
            m_geometryShaderFile = "";
            m_fragmentShaderFile = "";
        }
        
        Shader::~Shader()
        {
            clearShader();
        }
        
        void Shader::createFromString(std::string t_vertex, std::string t_frag)
        {
            m_vertexShader = t_vertex;
            m_fragmentShader = t_frag;

            compileShader(m_vertexShader, m_fragmentShader);
        }
        
        void Shader::createFromString(std::string t_vertex, std::string t_geom, std::string t_frag)
        {
            m_vertexShader = t_vertex;
            m_geometryShader = t_geom;
            m_fragmentShader = t_frag;

            compileShader(m_vertexShader, m_geometryShader, m_fragmentShader);
        }
        
        void Shader::createFromFiles(std::string t_vertFile, std::string t_fragFile)
        {
            m_vertexShaderFile = t_vertFile;
            m_fragmentShaderFile = t_fragFile;

            m_vertexShader = readFile(m_vertexShaderFile);
            m_fragmentShader = readFile(m_fragmentShaderFile);
            
            compileShader(m_vertexShader, m_fragmentShader);
        }
        
        void Shader::createFromFiles(std::string t_vertFile, std::string t_geomFile, std::string t_fragFile)
        {
            m_vertexShaderFile = t_vertFile;
            m_geometryShaderFile = t_geomFile;
            m_fragmentShaderFile = t_fragFile;

            m_vertexShader = readFile(m_vertexShaderFile);
            m_geometryShader = readFile(m_geometryShaderFile);
            m_fragmentShader = readFile(m_fragmentShaderFile);
            
            compileShader(m_vertexShader, m_geometryShader, m_fragmentShader);
        }
        
        std::string Shader::readFile(std::string t_filepath)
        {
            std::string fullPath = "shaders/" + t_filepath;
            std::string content;
            std::ifstream fileStream(fullPath, std::ios::in);
            
            if (!fileStream.is_open())
            {
                CORE_CRITICAL("failed to read shader: {0}", fullPath);
                return "";
            }
            
            std::string line = "";
            while(!fileStream.eof())
            {
                std::getline(fileStream, line);
                content.append(line + "\n");
            }
            
            fileStream.close();
            
            CORE_INFO("shader read: {0}", fullPath);
            
            return content;
        }
        
        unsigned int Shader::getUniformLocation(std::string t_unifromId)
        {
            return m_uniformMap[t_unifromId];
        }
        
        void Shader::addUniform(std::string t_unifromId)
        {
            CORE_TRACE("added uniform '{0}'", t_unifromId);
            m_uniformMap[t_unifromId] = glGetUniformLocation(m_shaderId, t_unifromId.c_str());
        }
        
        void Shader::setUniform1f(std::string t_uniformId, float t_val)
        {
            CORE_TRACE("set uniform '{0}' with value '{1}'", t_uniformId, t_val);
            m_uniform1fMap[t_uniformId] = t_val;
        }
        
        void Shader::setUniform2f(std::string t_uniformId, glm::vec2 t_val)
        {
            CORE_TRACE("set uniform '{0}' with value ({1},{2})", t_uniformId, t_val.x, t_val.y);
            m_uniform2fMap[t_uniformId] = t_val;
        }
        
        void Shader::setUniform3f(std::string t_uniformId, glm::vec3 t_val)
        {
            CORE_TRACE("set uniform '{0}' with value ({1},{2},{3})", t_uniformId, t_val.x, t_val.y, t_val.z);
            m_uniform3fMap[t_uniformId] = t_val;
        }
        
        void Shader::setUniform4f(std::string t_uniformId, glm::vec4 t_val)
        {
            CORE_TRACE("set uniform '{0}' with value ({1},{2},{3},{4})", t_uniformId, t_val.x, t_val.y, t_val.z, t_val.w);
            m_uniform4fMap[t_uniformId] = t_val;
        }
        
        void Shader::useShader()
        {
            glUseProgram(m_shaderId);
            for (auto val : m_uniform1fMap)
            {
                unsigned int uniformLocation = getUniformLocation(val.first);
                glUniform1f(uniformLocation, val.second);
            }
            
            for (auto val : m_uniform2fMap)
            {
                unsigned int uniformLocation = getUniformLocation(val.first);
                glUniform2f(uniformLocation, val.second.x, val.second.y);
            }
            
            for (auto val : m_uniform3fMap)
            {
                unsigned int uniformLocation = getUniformLocation(val.first);
                glUniform3f(uniformLocation, val.second.x, val.second.y, val.second.z);
            }
            
            for (auto val : m_uniform4fMap)
            {
                unsigned int uniformLocation = getUniformLocation(val.first);
                glUniform4f(uniformLocation, val.second.x, val.second.y, val.second.z, val.second.w);
            }
            
            CORE_TRACE("shader with id '{0}' used", m_shaderId);
        }
        
        void Shader::clearShader()
        {
            if (m_shaderId != 0)
            {
                glDeleteProgram(m_shaderId);
                m_shaderId = 0;
            }
            
            m_uniformMap.clear();
        }
        
        void Shader::compileShader(std::string t_vertex, std::string t_frag)
        {
            m_shaderId = glCreateProgram();
            
            if (!m_shaderId)
            {
                CORE_ERROR("failed to create shader");
                return;
            }
            
            addShader(m_shaderId, t_vertex.c_str(), GL_VERTEX_SHADER);
            addShader(m_shaderId, t_frag.c_str(), GL_FRAGMENT_SHADER);
            
            int result = 0;
            char eLog[1024] = { 0 };
            
            glLinkProgram(m_shaderId);
            glGetProgramiv(m_shaderId, GL_LINK_STATUS, &result);
            if (!result)
            {
                glGetProgramInfoLog(m_shaderId, sizeof(eLog), NULL, eLog);
                CORE_ERROR("error compiling shader: {0}", eLog);
                return;
            }
            
            glValidateProgram(m_shaderId);
            glGetProgramiv(m_shaderId, GL_VALIDATE_STATUS, &result);
            if (!result)
            {
                glGetProgramInfoLog(m_shaderId, sizeof(eLog), NULL, eLog);
                CORE_ERROR("error compiling shader: {0}", eLog);
                return;
            }
            
            CORE_INFO("shader with id '{0}' created succsefully", m_shaderId);
            
            addUniform("u_Proj");
            addUniform("u_View");
            addUniform("u_TexData");
            addUniform("u_CamPos");
            addUniform("u_Time");
        }
        
        void Shader::compileShader(std::string t_vertex, std::string t_geometry, std::string t_frag)
        {
            m_shaderId = glCreateProgram();
            
            if (!m_shaderId)
            {
                CORE_ERROR("failed to create shader");
                return;
            }
            
            addShader(m_shaderId, t_vertex.c_str(), GL_VERTEX_SHADER);
            addShader(m_shaderId, t_geometry.c_str(), GL_GEOMETRY_SHADER);
            addShader(m_shaderId, t_frag.c_str(), GL_FRAGMENT_SHADER);
            
            int result = 0;
            char eLog[1024] = { 0 };
            
            glLinkProgram(m_shaderId);
            glGetProgramiv(m_shaderId, GL_LINK_STATUS, &result);
            if (!result)
            {
                glGetProgramInfoLog(m_shaderId, sizeof(eLog), NULL, eLog);
                CORE_ERROR("error compiling shader: {0}", eLog);
                return;
            }
            
            glValidateProgram(m_shaderId);
            glGetProgramiv(m_shaderId, GL_VALIDATE_STATUS, &result);
            if (!result)
            {
                glGetProgramInfoLog(m_shaderId, sizeof(eLog), NULL, eLog);
                CORE_ERROR("error compiling shader: {0}", eLog);
                return;
            }
            
            CORE_INFO("shader with id '{0}' created succsefully", m_shaderId);
            
            addUniform("u_Proj");
            addUniform("u_View");
            addUniform("u_TexData");
            addUniform("u_CamPos");
            addUniform("u_Time");
        }
        
        void Shader::addShader(unsigned int t_program, const char *t_shaderCode, GLenum t_shaderType)
        {
            unsigned int tShader = glCreateShader(t_shaderType);
            
            const char* code[1];
            code[0] = t_shaderCode;
            
            int codeLenagh[1];
            codeLenagh[0] = (int)std::strlen(t_shaderCode);
            
            glShaderSource(tShader, 1, code, codeLenagh);
            glCompileShader(tShader);
            
            int result = 0;
            char eLog[1024] = { 0 };
            
            glGetShaderiv(tShader, GL_COMPILE_STATUS, &result);
            if (!result)
            {
                glGetShaderInfoLog(tShader, sizeof(eLog), NULL, eLog);
                CORE_ERROR("error compiling the {0} in shader: {1}", t_program, eLog);
                return;
            }
            
            CORE_INFO("created {0} shader program sucsseuflly", t_shaderType);
            
            glAttachShader(t_program, tShader);
        }
    }
}
