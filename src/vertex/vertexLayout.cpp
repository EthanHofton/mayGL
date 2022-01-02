#include <vertex/vertexLayout.hpp>

namespace may
{
    namespace vertex
    {
        VertexLayout::VertexLayout()
        {
            m_vertexStride = 0;
        }
        
        VertexLayout::~VertexLayout()
        {
            for (auto vertex : m_vertexComponents)
            {
                delete vertex;
                vertex = nullptr;
            }
            
            m_vertexComponents.clear();
        }
        
        void VertexLayout::push(VertexComponent *t_v)
        {
            m_vertexComponents.push_back(t_v);
            m_vertexStride += t_v->getVertexComponentStride() * t_v->getCount();
        }
        
        void VertexLayout::init()
        {
            int offset = 0;
        
            for (int i = 0; i < m_vertexComponents.size(); i++)
            {
                auto vertexComponent = m_vertexComponents[i];
                glEnableVertexAttribArray(i);
                glVertexAttribPointer(i, vertexComponent->getCount(), vertexComponent->getDataType(), vertexComponent->isNormalised(), getVertexStride(), (void*)(uintptr_t)(offset));
                offset += vertexComponent->getCount() * vertexComponent->getVertexComponentStride();
            }
        }
    }
}
