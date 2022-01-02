#ifndef textureAtlasComponent_hpp
#define textureAtlasComponent_hpp

#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <rapidjson/document.h>

#include "component.hpp"

using namespace rapidjson;

namespace may
{
    namespace component
    {
        class Mesh;
    
        class TextureAtlas : public Component
        {
        public:

            TextureAtlas(entity::Entity *t_parent, std::string t_id, std::string t_jsonPath, std::string t_atlasId, int t_vstart = -1, int t_vend = -1);
            ~TextureAtlas() = default;

            void setTextureCoords();

            inline std::string getJsonPath() { return m_jsonPath; }
            inline std::string getAtlasId() { return m_atlasId; }

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();

            inline void update() override {}

        private:

            std::string getFile(std::string t_filename);

        private:

            std::string m_jsonPath;
            std::string m_atlasId;

            glm::mat2 m_coordMat;

            Document m_jsonConfig;

            int m_vertexStart;
            int m_vertexEnd;

            std::vector<Mesh*> m_meshes;
        };
    }
}

#endif