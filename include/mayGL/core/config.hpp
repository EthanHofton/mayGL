#ifndef config_hpp
#define config_hpp

#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <rapidjson/document.h>

using namespace rapidjson;

namespace may
{
    namespace config
    {
        class Config
        {
        public:
            
            static Config* instance();
            static void release();
            
        private:
            
            Config();
            ~Config() = default;
            
        public:
            
            inline Document &getLoggerConifg() { return m_loggerConifg; }
            inline Document &getGameConifg() { return m_gameConfig; }
            
            // fequetly used varibles
            inline int getWidth() { return m_gameConfig["width"].GetInt(); }
            inline int getHeight() { return m_gameConfig["height"].GetInt(); }
            
        private:
            
            std::string getFile(std::string t_filename);
            
        private:
            
            static Config* s_instance;
            
            Document m_loggerConifg;
            Document m_gameConfig;

        };
    }
}

// config macros
#define CONFIG() config::Config::instance()
#define LOGGER_CONFIG() config::Config::instance()->getLoggerConifg()
#define GAME_CONFIG() config::Config::instance()->getGameConifg()

#endif