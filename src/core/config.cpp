#include <mayGL/core/config.hpp>

namespace mayGL
{
    namespace config
    {
        Config *Config::s_instance = nullptr;
        
        Config *Config::instance()
        {
            if (s_instance == nullptr)
            {
                s_instance = new Config;
            }
            
            return s_instance;
        }
        
        void Config::release()
        {
            if (s_instance != nullptr)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }
        
        Config::Config()
        {
            // TODO: use user defined macro
            std::string loggerConfigFile = "config/loggerConfig.json";
            std::string gameConfigFile = "config/gameConfig.json";
            
            m_loggerConifg.Parse(getFile(loggerConfigFile).c_str());
            m_gameConfig.Parse(getFile(gameConfigFile).c_str());
        }
        
        std::string Config::getFile(std::string t_filename)
        {
            std::string line;
            std::string output;
            std::ifstream file(t_filename);
            
            assert(file.is_open());
            
            while (std::getline(file, line))
            {
                output.append(line);
            }
            
            return output;
        }
    }
}
