#ifndef colorComponent_hpp
#define colorComponent_hpp

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

        class ColorComponent : public Component
        {
            
        public:
            
            ColorComponent(entity::Entity *t_parent, std::string t_id, glm::vec4 t_color, int t_start = -1, int t_end = -1);
            ColorComponent(entity::Entity *t_parent, std::string t_id, glm::vec3 t_color, int t_start = -1, int t_end = -1);
            ColorComponent(entity::Entity *t_parent, std::string t_id, std::string t_hex, int t_start = -1, int t_end = -1);
            
            ~ColorComponent() = default;
            
            inline glm::vec4 getColor() { return m_color; }
            inline glm::vec3 getRGB() { return glm::vec3(m_color.r, m_color.g, m_color.b); }
            inline std::string getHex() { return color::rgbToHex(m_color); }
            inline float getAlpha() { return m_color.a; }

            void setAlpha(float t_alpha);
            void setRGB(glm::vec3 t_rgb);
            void setColor(glm::vec4 t_color);
            void setColor(std::string t_hex, float t_alpha = 1.0f);

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();

            void updateColor();

            void update() override {}
            
        private:
            
            glm::vec4 m_color;
            int m_vertexStart;
            int m_vertexEnd;

            std::vector<Mesh*> m_meshes;
        };
    }
}

#endif