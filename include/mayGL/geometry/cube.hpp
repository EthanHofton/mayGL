#ifndef cube_hpp
#define cube_hpp

#include "../entity/entity.hpp"

namespace mayGL
{
    namespace geometry
    {
        struct cube_vertex
        {
            glm::vec3 m_pos;
            glm::vec4 m_color;
            glm::vec2 m_texCoords;
        };
        
        class Cube : public entity::Entity
        {
        public:
            
            Cube(glm::vec3 t_size);
            ~Cube() {}

            component::Mesh *getCubeMesh();
            component::Transform *getCubeMeshTransform();

            inline std::string getCubeMeshId() { return m_cubeMeshId; }
            inline std::string getCubeMeshTrnasformId() { return m_cubeMeshTransformId; }
            
            inline glm::vec3 getSize() { return m_size; }
            
        private:
            
            std::vector<cube_vertex> m_vertexData;
            std::vector<unsigned int> m_indexData;

            std::string m_cubeMeshId;
            std::string m_cubeMeshTransformId;
            
            glm::vec3 m_size;
            
        };
    }
}


#endif