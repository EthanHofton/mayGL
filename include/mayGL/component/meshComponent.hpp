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
            
            Mesh(entity::Entity *t_parent, std::string t_id, vertex::VertexLayout *t_layout);
            ~Mesh();
            
            inline void *getIndices() { return m_indices; }
            inline void *getWorldVertices() { return m_worldVertices; }
            inline void *getModelVertices() { return m_modelVertices; }
            
            void setVertices(void *t_vertices, unsigned int t_vSize);
            void setIndices(void *t_indices, unsigned int t_iSize);
            
            inline vertex::VertexLayout *getLayout() { return m_vertexLayout; }
            
            inline bool isVisable() { return m_isVisable; }
            inline void visable(bool t_b) { m_isVisable = t_b; }
            
            inline unsigned int getVerticesSize() { return m_verticesSize; }
            inline unsigned int getIndicesSize() { return m_indeicesSize; }
            
            inline void update() override {}

            inline std::vector<Texture*> getTextures() { return m_textures; }
            void addTexture(std::string t_texId);
            void addTextures(std::vector<std::string> t_texIds);
            void addAllTextures();
            void removeTexture(std::string t_texId);
            void removeAllTextures();

            void imguiComponentInspector() override;
            
        private:
                        
            void* m_worldVertices;
            void* m_indices;
            void* m_modelVertices;
            
            vertex::VertexLayout *m_vertexLayout;
            bool m_isVisable;
            
            unsigned int m_verticesSize; // BYTES
            unsigned int m_indeicesSize; // BYTES

            std::vector<Texture *> m_textures;
            
        };
    }
}

#endif