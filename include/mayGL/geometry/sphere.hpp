#ifndef sphere_hpp
#define sphere_hpp

#include "../support/support.hpp"
#include "../entity/entity.hpp"
#include "../component/meshComponent.hpp"
#include "../component/transformComponent.hpp"

namespace mayGL
{
    namespace geometry
    {
        struct sphere_vertex
        {
            glm::vec3 m_pos;
            glm::vec4 m_color;
            glm::vec3 m_normals;
            glm::vec2 m_textureCoords;
        };
        
        class Sphere : public entity::Entity
        {
        public:
            
            Sphere(float t_r);
            ~Sphere() {}
            
            inline float getRadius() { return m_radius; }

            component::Mesh *getSphereMesh();
            component::Transform *getSphereMeshTransform();

            inline std::string getSphereMeshId() { return m_meshId; }
            inline std::string getSphereMeshTrnasformId() { return m_meshTransformId; }
            
        private:
            
            std::vector<sphere_vertex> m_vertexData;
            std::vector<unsigned int> m_indexData;
            
            float m_radius;

            std::string m_meshId;
            std::string m_meshTransformId;
            
        };
    }
}

#endif