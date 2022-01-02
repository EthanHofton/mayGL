#ifndef vertexComponent_hpp
#define vertexComponent_hpp

#include <iostream>
#include <GL/glew.h>

namespace may
{
    namespace vertex
    {
        enum VertexComponentTypes
        {
            position,
            color,
            texture_coords,
            texture_unit,
            normals,
            custom
        };
        
        class VertexComponent
        {
        public:
            
            VertexComponent(std::string t_id, VertexComponentTypes t_type, int t_count, GLenum t_dataType, GLboolean t_normalised, int t_offset);
            ~VertexComponent();
            
            inline std::string getId() { return m_id; }
            inline VertexComponentTypes getType() { return m_vertexType; }
            
            inline int getCount() { return m_count; }
            inline GLenum getDataType() { return m_dataType; }
            inline GLboolean isNormalised() { return m_normalised; }
            inline int getVertexOffset() { return m_vertexOffset; }
            inline int getVertexComponentStride() { return m_vertexComponentStride; }
            
        private:
            
            int m_vertexComponentStride; // IN BYTES
            int m_vertexOffset; // IN BYTES
            int m_count; // components per attribute
            GLenum m_dataType;
            GLboolean m_normalised;
            
            std::string m_id;
            VertexComponentTypes m_vertexType;
        };
    }
}

#endif