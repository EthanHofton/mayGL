#include <mayGL/core/window.hpp>

namespace mayGL
{
    namespace core
    {
        Window::Window(std::string t_title)
        {
            m_graphicsInstance = Graphics::instance();
            m_graphicsInstance->title(t_title);
            m_graphicsInstance->clearColor(glm::vec4(1,1,1,1));
            m_graphicsInstance->init();
            
            m_isRunning = m_graphicsInstance->initalised();
            
            config::Config::instance();
            
            m_activePage = nullptr;
            m_timerInstance = Timer::instance();
            
            m_frameRate = (float)(GAME_CONFIG()["fps"].GetInt());
            m_inputManager = InputManager::instance();

            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);
            
            logger::Logger::instance();
        }
        
        Window::~Window()
        {
            Graphics::release();
            m_graphicsInstance = nullptr;
            
            Timer::release();
            m_timerInstance = nullptr;
            
            InputManager::release();
            m_inputManager = nullptr;

            glDeleteBuffers(1, &m_vao);
            
            for (auto page : m_pageStack)
            {
                page->onDelete();
                delete page;
                page = nullptr;
            }
            
            m_activePage = nullptr;
            
            // make sure all entites are deleted
            entity::Entity::deleteAllEntities();
            
            logger::Logger::release();
            config::Config::release();
            
        }
        
        void Window::setActivePage(std::string t_pageId)
        {
            CORE_INFO("setting active page with id: {0}", t_pageId);
            m_activePage = Page::s_allPages[t_pageId];
        }
        
        void Window::addPage(Page *t_page)
        {
            t_page->onCreate();
            
            if (m_activePage == nullptr)
            {
                m_activePage = t_page;
            }
            
            CORE_INFO("adding page with id '{0}'", t_page->getPageID());
            m_pageStack.push_back(t_page);
        }
        
        void Window::addPage(std::string t_pageId)
        {
            if (Page::s_allPages[t_pageId] != nullptr)
            {
                addPage(Page::s_allPages[t_pageId]);
            }
        }
        
        void Window::run()
        {
            while (m_isRunning)
            {
                m_timerInstance->tick();
                
                if (m_timerInstance->getDeltaTime() >= (1.0 / m_frameRate))
                {
                    while (m_graphicsInstance->pollEvent())
                    {
                        ImGui_ImplSDL2_ProcessEvent(&m_graphicsInstance->getEvent());
                        m_inputManager->giveEvents(m_graphicsInstance->getEvent());
                        
                        if (m_activePage != nullptr)
                        {
                            m_activePage->onEvent(m_graphicsInstance->getEvent());
                        }
                        
                        if (m_graphicsInstance->getEvent().type == SDL_QUIT)
                        {
                            m_isRunning = false;
                            break;
                        }
                    }
                    
                    // early update
                    earlyUpdate();
                    // early update
                    
                    // update
                    update();
                    // update
                    
                    // render
                    draw();
                    // render
                    
                    // late update
                    lateUpdate();
                    // late update
                }
            }
        }

        void Window::earlyUpdate()
        {
            m_timerInstance->reset();
            m_inputManager->update();
        }
        
        void Window::update()
        {
            if (m_activePage != nullptr)
            {
                m_activePage->onUpdate();
                m_activePage->updateEntities();
            }
        }

        void Window::lateUpdate()
        {
            m_inputManager->updatePrevInput();
        }
        
        void Window::draw()
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            m_graphicsInstance->screenClear();
            m_graphicsInstance->screenClearColor();
            
            if (m_activePage != nullptr)
            {
                m_activePage->onDraw();
            }
            
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
            m_graphicsInstance->swapBuffers();
        }
    }
}
