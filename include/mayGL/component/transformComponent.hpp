#ifndef transformComponent_hpp
#define transformComponent_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "component.hpp"
#include "../support/support.hpp"

namespace mayGL
{
    namespace component
    {
        enum space
        {
            model,
            world
        };

        class Mesh;
        
        class Transform : public Component
        {
        public:
            
            Transform(entity::Entity *t_parent, std::string t_id);
            ~Transform();
            
            void rotate(float t_rad, glm::vec3 t_axis);
            void setRotation(float t_rad, glm::vec3 t_axis);
            float getRoatation(space t_s = world);
            
            void scale(glm::vec3 t_scale);
            void setScale(glm::vec3 t_scale);
            glm::vec3 getScale(space t_s = world);
            
            void pos(glm::vec3 t_pos);
            void translate(glm::vec3 t_translation);
            glm::vec3 getPos(space t_s = world);
            
            inline glm::mat4 getModelMatrix() { return m_modelMatrix; }
            inline glm::mat4 getTranslationMatrix() { return m_translationMatrix; }
            inline glm::mat4 getScaleMatrix() { return m_scaleMatrix; }
            inline glm::mat4 getRoatationMatrix() { return m_rotationMatrix; }
            
            inline bool needsTransformUpdate() { return (m_transformUpdate || m_needsMatrixUpdate); }

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();
            
            void update() override;
            
        private:
            
            glm::mat4 m_scaleMatrix;
            glm::mat4 m_translationMatrix;
            glm::mat4 m_rotationMatrix;
            glm::mat4 m_modelMatrix;
            
            glm::vec3 m_pos;
            glm::vec3 m_scale;
            float m_roatation;
            
            bool m_needsMatrixUpdate;
            bool m_transformUpdate;

            std::vector<Mesh*> m_meshes;
        };
    }
}

#endif