#include <mayGL/component/textureComponent.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace component
    {
        std::map<std::string, std::pair<int, int>> Texture::s_textureMap;

        Texture::Texture(entity::Entity *t_parent, std::string t_id, glm::vec2 t_size) : Component(t_parent, texture, t_id)
        {
            m_filename = "";
            m_size = t_size;
            m_bitDepth = 0;
            m_previewImageArea = 4096;
            glGenTextures(1, &m_textureId);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_FLOAT, (void*)nullptr);
        }
        
        Texture::Texture(entity::Entity *t_parent, std::string t_id, std::string t_filename, std::string t_uniformId) : Component(t_parent, texture, t_id)
        {
            m_filename = t_filename;
            std::string filepath = m_filename;
            m_previewImageArea = 4096;

            m_uniformId = (t_uniformId == "") ? (getParent()->getEntityId() + "texture") : t_uniformId;
            
            int width, height;
            
            stbi_set_flip_vertically_on_load(false);
            unsigned char *texData = stbi_load(filepath.c_str(), &width, &height, &m_bitDepth, 0);
            m_size.x = width;
            m_size.y = height;
            
            if (s_textureMap.find(m_filename) == s_textureMap.end())
            {
                if (!texData)
                {
                    CORE_ERROR("error finding filename: {}", filepath);
                    assert(texData);
                }
                
                glGenTextures(1, &m_textureId);
                glBindTexture(GL_TEXTURE_2D, m_textureId);
                
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.x, m_size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
                glGenerateMipmap(GL_TEXTURE_2D);
                
                stbi_image_free(texData);
                
                s_textureMap[m_filename] = {m_textureId, 1};
            } else {
                m_textureId = s_textureMap[m_filename].first;
                s_textureMap[m_filename].second++;
                
                stbi_image_free(texData);
            }
        }
        
        Texture::~Texture()
        {
            if (m_filename == "") 
            {
                glDeleteTextures(1, &m_textureId);
            }

            if (s_textureMap[m_filename].second != 1) 
            {
                s_textureMap[m_filename].second--;
            } else {
                s_textureMap.erase(s_textureMap.find(m_filename));
                glDeleteTextures(1, &m_textureId);
            }
        }
        
        void Texture::use(int t_unit)
        {
            glActiveTexture(GL_TEXTURE0 + t_unit);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
        }

        void Texture::imguiComponentInspector()
        {
            // Component Info
            Component::imguiComponentInspector();

            // unique id so multiple tex components dont get confused
            std::string uidPrefix = "##" + m_id + getParent()->getEntityId();

            // image properties header
            if (ImGui::TreeNodeEx(("Image properties" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // filename label
                ImGui::LabelText("m_filename", "%s", m_filename.c_str());

                // size label
                ImGui::LabelText("m_size", "%ix%ipx", (int)m_size.x, (int)m_size.y);

                // bit depth label
                ImGui::LabelText("m_bitDepth", "%i", m_bitDepth);

                // image preview
                ImGui::Text("Image preview");
                glm::vec2 size = calculateResolution(m_previewImageArea, m_size);
                ImGui::Image((void*)(intptr_t)m_textureId, ImVec2(size.x, size.y));
                ImGui::DragInt("preview image area", &m_previewImageArea, 8);
            }

            ImGui::Separator();

            // image properties header
            if (ImGui::TreeNodeEx(("GLTexture properties" + uidPrefix).c_str(), ImGuiTreeNodeFlags_NoTreePushOnOpen))
            {
                // texId label
                ImGui::LabelText("m_textureId", "%i", m_textureId);

                // texId label
                ImGui::InputText("m_uniformId", &m_uniformId);
            }

            ImGui::Separator();
        }

        glm::vec2 Texture::calculateResolution(int t_area, glm::vec2 t_size)
        {
            float newWidth = sqrt((t_size.x / t_size.y) * t_area);
            float newHight = t_area / newWidth;

            int w = roundf(newWidth);
            int h = roundf(newHight);

            return {w,h};
        }
    }
}
