#ifndef model_hpp
#define model_hpp

#include <mayGL/entity/entity.hpp>
#include <mayGL/component/materialComponent.hpp>
#include <mayGL/component/textureComponent.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

namespace mayGL
{
    namespace geometry
    {
        using namespace mayGL;

        struct modelVertex
        {
            glm::vec3 m_pos;
            glm::vec3 m_normal;
            glm::vec2 m_texCoords;
            glm::vec3 m_Ka = glm::vec3(1);
            glm::vec3 m_Kd = glm::vec3(1);
            glm::vec3 m_Ks = glm::vec3(1);
            float m_Ns = 32;
            float m_mapKd = 0;
            float m_mapKs = 0;
        };

        class Model : public entity::Entity
        {
        public:

            Model(std::string t_modelFilepath);
            ~Model() = default;

            inline void update() override {}

            inline std::vector<component::Mesh*> getModelMeshes() { return m_meshes; }
            inline component::Transform *getModelTransform() { return m_modelTransform; }

        private:

            void loadFromFile(std::string t_path);
            component::Texture* loadTexture(aiMaterial *t_mat, aiTextureType t_type, std::string t_matName);

        private:

            std::vector<component::Mesh*> m_meshes;
            component::Transform *m_modelTransform;

            std::string m_meshID;
            std::string m_transformID;
            std::string m_directory;

            std::string m_modelOBJFile;
        };
    }
}

#endif