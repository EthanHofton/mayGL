#ifndef model_hpp
#define model_hpp

#include <mayGL/entity/entity.hpp>
#include <mayGL/component/materialComponent.hpp>

#include <unordered_map>
#include <glm/gtx/hash.hpp>
#include <tinyobjloader/tiny_obj_loader.h>

namespace mayGL
{
    namespace geometry
    {
        using namespace mayGL;

        struct modelVertex
        {
            glm::vec3 m_pos;
            glm::vec3 m_normal;

            bool operator==(const modelVertex& other) const
            {
                return m_pos == other.m_pos && m_normal == other.m_normal;
            }
        };

        class Model : public entity::Entity
        {
        public:

            Model(std::string t_modelFilepath);
            ~Model();

            inline void update() override {}

            inline component::Mesh* getModelMesh() { return m_modelMesh; }
            inline component::Transform *getModelTransform() { return m_modelTransform; }
            inline std::string getModelTransformId() { return m_transformID; }
            inline std::string getModelMeshId() { return m_meshID; }

        private:

            void loadOBJ(std::string t_path, component::Mesh *t_mesh);

        private:

            component::Mesh *m_modelMesh;
            component::Transform *m_modelTransform;

            std::string m_meshID;
            std::string m_transformID;

            std::string m_modelOBJFile;
        };
    }
}

namespace std
{
    template <>
    struct hash<mayGL::geometry::modelVertex>
    {
        size_t operator()(const mayGL::geometry::modelVertex& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.m_pos) ^ (hash<glm::vec3>()(vertex.m_normal) << 1)) >> 1);
        }
    };
}

#endif