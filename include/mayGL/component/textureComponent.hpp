#ifndef textureComponent_hpp
#define textureComponent_hpp

#include <vector>
#include <glm/glm.hpp>

#include <stb_image/stb_image.hpp>

#include "component.hpp"
#include "../vertex/vertexLayout.hpp"
#include "../renderer/shaderManager.hpp"

namespace mayGL
{
    namespace component
    {
        class Mesh;

        class Texture : public Component
        {
        public:
            
            static std::map<std::string, std::pair<int, int>> s_textureMap;
            
        public:
            
            Texture(entity::Entity *t_parent, std::string t_id, std::string t_filename);
            ~Texture();
            
            inline unsigned int getTextureId() { return m_textureId; }
            inline int getBitDepth() { return m_bitDepth; }
            inline int getTextureUnit() { return m_textureUnit; }
            inline std::string getFileName() { return m_filename; }
            inline glm::vec2 getSize() { return m_size; }
            
            void setTextureUnit(unsigned int t_unit);
            void use();

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();
            
            void updateTextureUnit();
            void update() override {}
            
        private:
            
            std::string m_filename;
            unsigned int m_textureId;
            glm::vec2 m_size;
            int m_bitDepth;
            int m_textureUnit;
            
            bool m_updateTextureUnit;

            std::vector<Mesh*> m_meshes;
        };
    }
}

#endif