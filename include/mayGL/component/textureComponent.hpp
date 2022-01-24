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
            
            Texture(entity::Entity *t_parent, std::string t_id, std::string t_filename, std::string t_uniformId = "");
            ~Texture();
            
            inline unsigned int getTextureId() { return m_textureId; }
            inline int getBitDepth() { return m_bitDepth; }
            inline std::string getFileName() { return m_filename; }
            inline glm::vec2 getSize() { return m_size; }

            inline std::string getUniformId() { return m_uniformId; }
            inline void setUniformId(std::string t_id) { m_uniformId = t_id; }
            
            void use(int t_unit);
            
            void update() override {}

            void imguiComponentInspector() override;
            
        private:

            // imgui
            glm::vec2 calculateResolution(int t_area, glm::vec2 t_size);
            int m_previewImageArea;
            // imgui
            
            std::string m_filename;
            unsigned int m_textureId;
            glm::vec2 m_size;
            int m_bitDepth;

            std::string m_uniformId;
        };
    }
}

#endif