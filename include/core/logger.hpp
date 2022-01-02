#ifndef logger_hpp
#define logger_hpp

#include <iostream>
#include <memory>

#define FMT_HEADER_ONLY
#include <spdlog/spdlog.h>
#include <fmt/format.h>

#include "config.hpp"

namespace may
{
    namespace logger
    {
        class Logger
        {
        public:
            
            // singleton instance and releace functions
            static Logger* instance();
            static void release();
            
        private:
            
            // logger constructor
            Logger();
            
        public:
            
            // client logger functions
            template<typename... Args>
            inline void trace(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_clientFileLogger->trace(fmt, std::forward<Args>(args)...);
                m_clientConsoleLogger->trace(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void debug(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_clientFileLogger->debug(fmt, std::forward<Args>(args)...);
                m_clientConsoleLogger->debug(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void info(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_clientFileLogger->info(fmt, std::forward<Args>(args)...);
                m_clientConsoleLogger->info(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void warn(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_clientFileLogger->warn(fmt, std::forward<Args>(args)...);
                m_clientConsoleLogger->warn(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void error(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_clientFileLogger->error(fmt, std::forward<Args>(args)...);
                m_clientConsoleLogger->error(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void critical(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_clientFileLogger->critical(fmt, std::forward<Args>(args)...);
                m_clientConsoleLogger->critical(fmt, std::forward<Args>(args)...);
            }
            
            // core logger functions
            template<typename... Args>
            inline void coreTrace(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_coreFileLogger->trace(fmt, std::forward<Args>(args)...);
                m_coreConsoleLogger->trace(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void coreDebug(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_coreFileLogger->debug(fmt, std::forward<Args>(args)...);
                m_coreConsoleLogger->debug(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void coreInfo(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_coreFileLogger->info(fmt, std::forward<Args>(args)...);
                m_coreConsoleLogger->info(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void coreWarn(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_coreFileLogger->warn(fmt, std::forward<Args>(args)...);
                m_coreConsoleLogger->warn(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void coreError(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_coreFileLogger->error(fmt, std::forward<Args>(args)...);
                m_coreConsoleLogger->error(fmt, std::forward<Args>(args)...);
            }
            
            template<typename... Args>
            inline void coreCritical(fmt::format_string<Args...> fmt, Args &&...args)
            {
                m_coreFileLogger->critical(fmt, std::forward<Args>(args)...);
                m_coreConsoleLogger->critical(fmt, std::forward<Args>(args)...);
            }
            
        private:
            
            // class instance
            static Logger* s_instance;
            
            // loggers
            std::shared_ptr<spdlog::logger> m_coreFileLogger;
            std::shared_ptr<spdlog::logger> m_clientFileLogger;
            std::shared_ptr<spdlog::logger> m_coreConsoleLogger;
            std::shared_ptr<spdlog::logger> m_clientConsoleLogger;
        };
    }
}

// core logger macros
#define CORE_TRACE(...) may::logger::Logger::instance()->coreTrace(__VA_ARGS__)
#define CORE_DEBUG(...) may::logger::Logger::instance()->coreDebug(__VA_ARGS__)
#define CORE_INFO(...) may::logger::Logger::instance()->coreInfo(__VA_ARGS__)
#define CORE_WARN(...) may::logger::Logger::instance()->coreWarn(__VA_ARGS__)
#define CORE_ERROR(...) may::logger::Logger::instance()->coreError(__VA_ARGS__)
#define CORE_CRITICAL(...) may::logger::Logger::instance()->coreCritical(__VA_ARGS__)

// client logger macros
#define APP_TRACE(...) may::logger::Logger::instance()->trace(__VA_ARGS__)
#define APP_DEBUG(...) may::logger::Logger::instance()->debug(__VA_ARGS__)
#define APP_INFO(...) may::logger::Logger::instance()->info(__VA_ARGS__)
#define APP_WARN(...) may::logger::Logger::instance()->warn(__VA_ARGS__)
#define APP_ERROR(...) may::logger::Logger::instance()->error(__VA_ARGS__)
#define APP_CRITICAL(...) may::logger::Logger::instance()->critical(__VA_ARGS__)

#endif