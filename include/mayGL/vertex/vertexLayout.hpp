#ifndef vertexLayout_hpp
#define vertexLayout_hpp

#include <iostream>
#include <vector>

#include "vertexComponent.hpp"

namespace mayGL
{
    namespace vertex
    {
        class VertexLayout
        {
        public:
            
            VertexLayout();
            ~VertexLayout();
            
            void push(VertexComponent *t_v);
            int getVertexStride() { return m_vertexStride; }

            bool hasComponent(VertexComponentTypes t_type);
            
            void init();
            
            inline std::vector<VertexComponent *> &getComponents() { return m_vertexComponents; }
            
        private:
            
            std::vector<VertexComponent *> m_vertexComponents;
            
            int m_vertexStride; // BYTES
        };
    }
}

#endif