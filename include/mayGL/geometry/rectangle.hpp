#ifndef rectangle_hpp
#define rectangle_hpp

#include "../entity/entity.hpp"
#include "../component/meshComponent.hpp"
#include "../component/transformComponent.hpp"

namespace mayGL
{
    namespace geometry
    {
        struct rectangle_vertex
        {
            glm::vec3 m_pos;
            glm::vec4 m_color;
            glm::vec2 m_texCoord;
        };
        
        class Rectangle : public entity::Entity
        {
        public:
            
            Rectangle(glm::vec2 t_size);
            ~Rectangle() {}
            
            inline glm::vec2 getSize() { return m_size; }

            component::Mesh *getRectMesh();
            component::Transform *getRectMeshTransform();

            inline std::string getRectMeshId() { return m_meshId; }
            inline std::string getRectMeshTrnasformId() { return m_meshTransformId; }
            
        private:
            
            std::vector<rectangle_vertex> m_vertexData;
            std::vector<unsigned int> m_indexData;
            
            glm::vec2 m_size;

            std::string m_meshId;
            std::string m_meshTransformId;
            
        };
    }
}

#endif