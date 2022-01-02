#include <core/page.hpp>

namespace may
{
    namespace core
    {
        std::map<std::string, Page*> Page::s_allPages;
        
        Page::Page(Window *t_parent, std::string t_pageId) : m_pageId(t_pageId), m_parent(t_parent)
        {
            CORE_INFO("creating page with id: {0}", t_pageId);
            s_allPages[m_pageId] = this;
        }
        
        Page::~Page()
        {
            auto instance = s_allPages.find(m_pageId);
            if (instance != s_allPages.end())
            {
                s_allPages.erase(instance);
            }
            
            CORE_INFO("deleteing page with id: {0}", m_pageId);
        }
        
        void Page::updateEntities()
        {
            for (auto entity : m_entities)
            {
                if (entity->isActive())
                {
                    entity->updateComponents();
                    entity->update();
                }
            }
        }
    }
}
