#ifndef meshComponent_hpp
#define meshComponent_hpp

#include <vector>
#include <glm/glm.hpp>
#include <sstream>
#include <string>

#include "component.hpp"
#include "../vertex/vertexLayout.hpp"
#include "../renderer/shaderManager.hpp"

namespace mayGL
{
    namespace component
    {
        class Texture;

        class Mesh : public Component
        {
        public:
            
            Mesh(entity::Entity *t_parent, std::string t_id, vertex::VertexLayout *t_layout, unsigned int t_primativeType);
            ~Mesh();
            
            void setShader(std::string t_shaderId);
            void loadShader(std::string t_vertexFile, std::string t_fragFile);
            void loadShader(std::string t_vertexFile, std::string t_geomFile, std::string t_fragFile);
            void loadShaderString(std::string t_vertex, std::string t_frag);
            void loadShaderString(std::string t_vertex, std::string t_geom, std::string t_frag);
            renderer::Shader *getShader();
            inline std::string getShaderId() { return m_shaderId; }
            inline bool hasShader() { return !(m_shaderId == ""); }
            
            inline void *getIndices() { return m_indices; }
            inline void *getWorldVertices() { return m_worldVertices; }
            inline void *getModelVertices() { return m_modelVertices; }
            
            void setVertices(void *t_vertices, unsigned int t_vSize);
            void setIndices(void *t_indices, unsigned int t_iSize);
            
            inline unsigned int getPrimativeType() { return m_primativeType; }
            inline vertex::VertexLayout *getLayout() { return m_vertexLayout; }
            
            inline bool isVisable() { return m_isVisable; }
            inline void visable(bool t_b) { m_isVisable = t_b; }
            
            inline unsigned int getVerticesSize() { return m_verticesSize; }
            inline unsigned int getIndicesSize() { return m_indeicesSize; }
            
            inline void update() override {}

            void setTexture(std::string t_textureId);
            inline Texture *getTexture() { return m_texture; }
            inline bool hasTexture() { return (m_texture != nullptr); }
            void removeTexture();

            void imguiComponentInspector() override;
            
        private:

            void writeShader(std::string t_filepath, std::string t_source);

            // imgui
            void imguiShaderEditor(std::string t_type, std::string& t_shaderText, bool &t_editing, bool &t_updated);
            void imguiShaderProperties(std::string t_type, std::string &t_sourceFile, std::string &t_source, bool &t_open);
            void imguiUpdateSources();
            std::string m_vertexSourceFilepath;
            std::string m_geometrySourceFilepath;
            std::string m_fragmentSourceFilepath;
            std::string m_vertexSource;
            std::string m_geometrySource;
            std::string m_fragmentSource;
            bool m_vertexSourceOpen;
            bool m_vertexSourceUpdated;
            bool m_geometrySourceOpen;
            bool m_geometrySourceUpdated;
            bool m_fragmentSourceOpen;
            bool m_fragmentSourceUpdated;
            // imgui
            
            renderer::ShaderManager *m_shaderManager;
            
            void* m_worldVertices;
            void* m_indices;
            void* m_modelVertices;
            
            std::string m_shaderId;
            
            unsigned int m_primativeType;
            vertex::VertexLayout *m_vertexLayout;
            bool m_isVisable;
            
            unsigned int m_verticesSize; // BYTES
            unsigned int m_indeicesSize; // BYTES

            Texture *m_texture;
            
        };
    }
}

#endif