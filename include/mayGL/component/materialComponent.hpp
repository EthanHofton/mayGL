#ifndef materialComponent_hpp
#define materialComponent_hpp

#include <vector>
#include <glm/glm.hpp>

#include "component.hpp"
#include "../vertex/vertexLayout.hpp"
#include "../support/support.hpp"

namespace mayGL
{
    namespace component
    {
        class Mesh;

        class Material : public Component
        {
            
        public:
            
            Material(entity::Entity *t_parent, std::string t_id, std::string t_materialName);            
            ~Material() = default;

            inline void setAmbient(glm::vec3 t_a) { m_ambient = t_a; }
            inline void setDiffuse(glm::vec3 t_d) { m_diffuse = t_d; }
            inline void setSpecular(glm::vec3 t_s) { m_specular = t_s; }
            inline void setShininess(float t_s) { m_shininess = t_s; }

            inline glm::vec3 getAmbient() { return m_ambient; }
            inline glm::vec3 getDiffuse() { return m_diffuse; }
            inline glm::vec3 getSpecular() { return m_specular; }
            inline float getShininess() { return m_shininess; }

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();

            void update() override;

            void imguiComponentInspector() override;
            
        private:

            void setShaderUniforms();

        private:
            
            glm::vec3 m_ambient;
            glm::vec3 m_diffuse;
            glm::vec3 m_specular;
            float m_shininess;

            std::string m_materialName;

            std::vector<Mesh*> m_meshes;
        };
    }
}

#endif