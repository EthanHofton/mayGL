#ifndef renderBatch_hpp
#define renderBatch_hpp

#include <iostream>
#include <vector>
#include <map>
#include <queue>

#include <glm/glm.hpp>

#define GLEW_STATIC
#include <GL/glew.h>

#include "shaderManager.hpp"
#include "../projection/baseProjection.hpp"
#include "../camera/camera.hpp"
#include "../vertex/vertexLayout.hpp"
#include "../core/config.hpp"

namespace mayGL
{
    namespace renderer
    {
        struct uboTextures
        {
            glm::vec4 m_textureMaps[32];
        }; // 512 bytes

        struct DrawCall
        {
            // -- VBO, VAO, IBO
            unsigned int m_VAO, m_VBO, m_IBO;
            // -- VBO, VAO, IBO
            
            // -- varibles
            int m_primativeType;
            vertex::VertexLayout *m_vertexLayout;
            std::string m_shaderId;
            std::string m_drawCallKey;
            bool m_full;
            int m_objects;
            unsigned int m_vertexDataOffset;
            unsigned int m_indexDataOffset;
            unsigned int m_indexValueOffset;
            // -- varibles
            
            // -- data
            void *m_vertexArray;
            void *m_indexArray;
            // -- data
            
            // -- texture
            std::map<std::string, int> m_boundTextures;
            std::vector<component::Texture *> m_textures;
            // uboTextures m_texturesUbo;
            // -- texture
        };
        
        class RenderBatch
        {
        public:
            
            RenderBatch();
            ~RenderBatch();
            
            void push(entity::Entity *t_e);
            
            void batchBegin();
            void batchEnd();
            void draw();

            // face culling for batch
            inline void enableFaceCulling() { m_cullFace = true; }
            inline void disableFaceCulling() { m_cullFace = false; }
            inline void faceCull(int t_face) { glCullFace(t_face); }

            // wireframe for batch
            inline void enableWireframe() { m_batchWireframe = true; }
            inline void disableWireframe() { m_batchWireframe = false; }
            
            // static/dynamic draws
            // static/dynamic draws
            
        private:
            
            void clearDrawCall(DrawCall &t_call);
            void resetDrawCall(DrawCall &t_call);
            void prepareDrawCall(DrawCall &t_call);
            void initDrawCall(DrawCall &t_call, int t_primativeType, vertex::VertexLayout *t_vertexLayout, std::string t_shaderId, std::string t_key);
            
        private:

            // face culling
            bool m_cullFace;

            // batch wireframe
            bool m_batchWireframe;
            
            std::vector<DrawCall> m_drawCalls;
            ShaderManager *m_shaderManager;
            
        };
    }
}

#endif