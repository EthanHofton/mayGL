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
            
            Material(entity::Entity *t_parent, std::string t_id);            
            ~Material() = default;

            inline void setAmbient(glm::vec3 t_a) { m_Ka = t_a; }
            inline void setDiffuse(glm::vec3 t_d) { m_Kd = t_d; }
            inline void setSpecular(glm::vec3 t_s) { m_Ks = t_s; }
            inline void setShininess(float t_s) { m_Ns = t_s; }
            inline void setDiffuseMapTextureId(float t_d) { m_mapKd = t_d; }
            inline void setSpecularMapTextureId(float t_s) { m_mapKs = t_s; }
            inline void setNormalMapTextureId(float t_b) { m_bump = t_b; }

            inline glm::vec3 getAmbient() { return m_Ka; }
            inline glm::vec3 getDiffuse() { return m_Kd; }
            inline glm::vec3 getSpecular() { return m_Ks; }
            inline float getShininess() { return m_Ns; }
            inline float getDiffuseMapTextureId() { return m_mapKd; }
            inline float getSpecularMapTextureId() { return m_mapKs; }
            inline float getNormalMapTextureId() { return m_bump; }

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();

            void update() override;

            void imguiComponentInspector() override;

        private:

            void setValue(component::Mesh *mesh, vertex::VertexComponentTypes t_type, float *t_data);

        private:

        // make Ka, Kd, Ks, Ns, map_Kd, map_Ks, bump vertex attributes
        // matieral class updates the values (stated above ^) per update.
        // EDIT: stopped passing material in as uniform to avoid unnessosary complication
            
            // imgui
            bool m_dragEdit;
            // imgui

            glm::vec3 m_Ka;
            glm::vec3 m_Kd;
            glm::vec3 m_Ks;
            float m_Ns;

            float m_mapKd;
            float m_mapKs;
            float m_bump;

            std::vector<Mesh*> m_meshes;
        };
    }
}

#endif