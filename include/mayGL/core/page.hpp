#ifndef page_hpp
#define page_hpp

#include <iostream>
#include <map>
#include <SDL2/SDL.h>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "logger.hpp"
#include "../entity/entity.hpp"

namespace mayGL
{
    namespace core
    {
        class Window;
        
        class Page
        {
        public:
            
            static std::map<std::string, Page*> s_allPages;
            
        public:
            
            Page(Window *t_parent, std::string t_pageId);
            virtual ~Page();
            
            inline virtual void onCreate() {}
            inline virtual void onDelete() {}
            inline virtual void onUpdate() {}
            inline virtual void onEvent(SDL_Event t_e) {}
            inline virtual void onDraw() {}
            
            inline std::string getPageID() { return m_pageId; }
            inline Window *getWindow() { return m_parent; }
            
            inline void pushEntity(entity::Entity *t_e)
            {
                m_entities.push_back(t_e);
                CORE_INFO("entity with id '{0}' added to page with id '{1}'", t_e->getEntityId(), m_pageId);
            }
            void updateEntities();

            inline std::vector<entity::Entity*> getEntites() { return m_entities; }

            void imguiShowScene();
            
        private:

            void addEntityPlaceholder(entity::Entity *t_e, int t_c);

        private:
            
            std::string m_pageId;
            Window *m_parent;
            
            std::vector<entity::Entity*> m_entities;
        };
    }
}

#endif