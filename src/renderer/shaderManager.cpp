#include <mayGL/renderer/shaderManager.hpp>

namespace may
{
    namespace renderer
    {
        ShaderManager *ShaderManager::s_instance = nullptr;
        
        ShaderManager *ShaderManager::instance()
        {
            if (s_instance == nullptr)
            {
                s_instance = new ShaderManager;
            }
            
            return s_instance;
        }
        
        void ShaderManager::release()
        {
            if (s_instance != nullptr)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }
        
        ShaderManager::ShaderManager() {}
        
        ShaderManager::~ShaderManager()
        {
            for (auto shader : m_shaders)
            {
                delete shader.second;
                shader.second = NULL;
            }
            
            m_shaders.clear();
        }
        
        std::string ShaderManager::loadFromFile(std::string t_vertex, std::string t_fragment)
        {
            std::string id = t_vertex + t_fragment;
            if (m_shaders[id] == NULL)
            {
                Shader *tmp = new Shader();
                tmp->createFromFiles(t_vertex, t_fragment);
                
                m_shaders[id] = tmp;
            }
            
            return id;
        }
        
        std::string ShaderManager::loadFromFile(std::string t_vertex, std::string t_geometry, std::string t_fragment)
        {
            std::string id = t_vertex + t_geometry + t_fragment;
            if (m_shaders[id] == NULL)
            {
                Shader *tmp = new Shader();
                tmp->createFromFiles(t_vertex, t_geometry, t_fragment);
                
                m_shaders[id] = tmp;
            }
            
            return id;
        }
        
        std::string ShaderManager::loadFromText(std::string t_vertex, std::string t_fragment)
        {
            std::string id = t_vertex + t_fragment;
            if (m_shaders[id] == NULL)
            {
                Shader *tmp = new Shader();
                tmp->createFromString(t_vertex, t_fragment);
                
                m_shaders[id] = tmp;
            }
            
            return id;
        }
        
        std::string ShaderManager::loadFromText(std::string t_vertex, std::string t_geometry, std::string t_fragment)
        {
            std::string id = t_vertex + t_geometry + t_fragment;
            if (m_shaders[id] == NULL)
            {
                Shader *tmp = new Shader();
                tmp->createFromString(t_vertex, t_geometry, t_fragment);
                
                m_shaders[id] = tmp;
            }
            
            return id;
        }
        
        Shader *ShaderManager::getShader(std::string t_shaderId)
        {
            return m_shaders[t_shaderId];
        }
        
        std::string ShaderManager::shaderID(Shader *t_s)
        {
            for (auto x: m_shaders)
            {
                if (x.second == t_s)
                    return x.first;
            }
            
            return nullptr;
        }
        
        void ShaderManager::addUniform(std::string t_shaderId, std::string t_unifromId)
        {
            if (m_shaders[t_shaderId] != nullptr)
            {
                m_shaders[t_shaderId]->addUniform(t_unifromId);
            }
        }
        
        unsigned int ShaderManager::uniformLocation(std::string t_shaderId, std::string t_unifromId)
        {
            return m_shaders[t_shaderId]->getUniformLocation(t_unifromId);
        }
    }
}
