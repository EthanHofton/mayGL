#ifndef timer_hpp
#define timer_hpp

#include <iostream>
#include <chrono>
#include "logger.hpp"

namespace may
{
    namespace core
    {
        class Timer
        {
        public:
            
            static Timer* instance();
            static void release();
            
        private:
            
            Timer();
            ~Timer() { CORE_INFO("timer deleted"); }
            
        public:
            
            void tick();
            
            void reset();
            inline double getDeltaTime() { return m_deltaTime.count(); }
            
            inline double getTimeScale() { return m_timeScale; }
            inline void timeScale(double t_t) { m_timeScale = t_t; }
            
        private:
        
            std::chrono::high_resolution_clock::time_point m_startTime;
            std::chrono::duration<double> m_deltaTime;
            
            double m_timeScale;
            
            static Timer* s_instance;
        };
    }
}


#endif