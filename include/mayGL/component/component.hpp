#ifndef component_hpp
#define component_hpp

#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "../core/logger.hpp"

namespace mayGL
{
    namespace entity
    {
        class Entity;
    }
    
    namespace component
    {
        enum component_types
        {
            transform,
            mesh,
            color,
            texture,
            textureAtlas,
            physicsBody,
            collider,
            material,
            custom
        };
        
        class Component
        {
        public:
            
            inline Component(entity::Entity *t_parent, component_types t_type, std::string t_id) : m_id(t_id), m_parent(t_parent), m_type(t_type)
            { CORE_INFO("component with id '{0}' created", m_id); }
            inline virtual ~Component()
            { m_parent = nullptr; CORE_INFO("component with id '{0}' deleted", m_id); }
            
            inline entity::Entity *getParent() { return m_parent; }

            inline std::string getId() { return m_id; }
            inline component_types getType() { return m_type; }

            template<typename T>
            T* as()
            {
                return static_cast<T*>(this);
            }
            
            virtual void imguiComponentInspector();
            virtual void update() = 0;
            
        protected:
            
            std::string m_id;
            entity::Entity *m_parent;
            component_types m_type;
        };
    }
}


#endif