#include <mayGL/vertex/vertexComponent.hpp>

namespace mayGL
{
    namespace vertex
    {
        VertexComponent::VertexComponent(std::string t_id, VertexComponentTypes t_type, int t_count, GLenum t_dataType, GLboolean t_normalised, int t_offset)
        {
            m_id = t_id;
            m_vertexType = t_type;
            m_dataType = t_dataType;
            m_count = t_count;
            m_normalised = t_normalised;
            m_vertexOffset = t_offset;
            
            switch (m_dataType)
            {
                case GL_BYTE:
                    m_vertexComponentStride = 1;
                    break;
                case GL_UNSIGNED_BYTE:
                    m_vertexComponentStride = 1;
                    break;
                case GL_SHORT:
                    m_vertexComponentStride = 2;
                    break;
                case GL_UNSIGNED_SHORT:
                    m_vertexComponentStride = 2;
                    break;
                case GL_INT:
                    m_vertexComponentStride = 4;
                    break;
                case GL_UNSIGNED_INT:
                    m_vertexComponentStride = 4;
                    break;
                case GL_HALF_FLOAT:
                    m_vertexComponentStride = 2;
                    break;
                case GL_FLOAT:
                    m_vertexComponentStride = 4;
                    break;
                case GL_DOUBLE:
                    m_vertexComponentStride = 8;
                    break;
            }
        }
        
        VertexComponent::~VertexComponent() {}
    }
}
