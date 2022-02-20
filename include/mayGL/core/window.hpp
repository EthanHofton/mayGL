#ifndef window_hpp
#define window_hpp

#include <iostream>
#include "graphics.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "page.hpp"
#include "timer.hpp"
#include "inputManager.hpp"
#include "../entity/entity.hpp"

namespace mayGL
{
    namespace core
    {
        class Window
        {
        public:
            
            Window(std::string t_title);
            ~Window();
            
            void run();
            void setActivePage(std::string t_pageId);
            void addPage(Page *t_page);
            void addPage(std::string t_pageId);
            
        private:
            
            void earlyUpdate();
            void update();
            void lateUpdate();
            void draw();
            
        private:
            
            Page* m_activePage;
            Graphics *m_graphicsInstance;
            Timer *m_timerInstance;
            
            std::vector<Page*> m_pageStack;
            
            float m_frameRate;
            InputManager *m_inputManager;
            unsigned int m_vao;
            
            bool m_isRunning;
        };
        
        Window *createWindow();
    }
}

#endif