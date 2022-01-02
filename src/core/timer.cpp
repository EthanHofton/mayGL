#include <mayGL/core/timer.hpp>

namespace may
{
    namespace core
    {
        Timer* Timer::s_instance = nullptr;
        
        Timer* Timer::instance()
        {
            if (s_instance == nullptr)
            {
                s_instance = new Timer();
            }
            
            return s_instance;
        }
        
        void Timer::release()
        {
            if (s_instance != nullptr)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }
        
        Timer::Timer()
        {
            reset();
            m_timeScale = 1.0;
            m_deltaTime = std::chrono::duration<double>(0.0);
            
            CORE_INFO("timer initalised");
        }
        
        void Timer::tick()
        {
            m_deltaTime = std::chrono::high_resolution_clock::now() - m_startTime;
        }
        
        void Timer::reset()
        {
            m_startTime = std::chrono::high_resolution_clock::now();
        }
    }
}
