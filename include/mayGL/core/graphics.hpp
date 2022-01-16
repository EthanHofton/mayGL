#ifndef graphics_hpp
#define graphics_hpp

#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "logger.hpp"
#include "config.hpp"

namespace mayGL
{
    namespace core
    {
        class Graphics
        {
        public:
            
            static Graphics* instance();
            static void release();
            
        private:
            
            static Graphics* s_instance;
            
            Graphics();
            ~Graphics();
            
        public:
            
            inline void title(std::string t_title) { m_title = t_title; }
            inline std::string getTitle() { return m_title; }
            
            inline void clearColor(std::string t_color) { /* TODO: fix */ m_color = {1,1,1,1};  }
            inline void clearColor(glm::vec4 t_color) { m_color = t_color; }
            inline glm::vec4 getClearColor() { return m_color; }
            
            inline void pos(glm::vec2 t_pos) { m_pos = t_pos; }
            inline glm::vec2 getPos() { return m_pos; }
            
            void init();
            inline bool initalised() { return m_initalised; }
            
            inline void destroyWindow() { SDL_DestroyWindow(m_window); CORE_INFO("destroied window"); }
            inline void deleteContext() { SDL_GL_DeleteContext(m_context); CORE_INFO("deleted OpenGL context"); }
            
            inline SDL_GLContext getGLContext() { return m_context; }
            
            inline void swapBuffers() { SDL_GL_SwapWindow(m_window); }
            inline int pollEvent() { m_event = SDL_Event(); return SDL_PollEvent(&m_event); }
            inline SDL_Event &getEvent() { return m_event; }
            
            inline void screenClearColor()
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
            inline void screenClear() { glClearColor(m_color.r, m_color.g, m_color.b, m_color.a); }
            
            inline bool wireframe() { return m_wireframe; }
            inline void wireframe(bool t_b)
            {
                m_wireframe = t_b;
                glPolygonMode(GL_FRONT_AND_BACK, (m_wireframe) ? GL_LINE : GL_FILL);
                CORE_INFO("wireframe mode {0}", (m_wireframe) ? "enabled" : "disabled");
            }
            
            inline bool relativeMouse() { return m_relativeMouse; }
            inline void relativeMouse(bool t_b)
            {
                m_relativeMouse = t_b;
                SDL_SetRelativeMouseMode((m_relativeMouse) ? SDL_TRUE : SDL_FALSE);
                CORE_INFO("relative mouse mode {0}", (m_relativeMouse) ? "enabled" : "disabled");
            }
            
        private:
            
            std::string m_title;
            glm::vec4 m_color;
            glm::vec2 m_pos;
            
            bool m_wireframe;
            bool m_relativeMouse;
            
            bool m_initalised;
            
            SDL_Window *m_window;
            SDL_GLContext m_context;
            SDL_Event m_event;
            
        };
    }
}

#endif