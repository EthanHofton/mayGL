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
            std::map<std::string, int> m_boundTextures; // filepath : textureUnit
            std::vector<component::Texture *> m_textures;
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
            void draw(glm::mat4 &t_view, glm::mat4 &t_projection, glm::vec3 t_camPos, float t_time);
            
            // static/dynamic draws
            // static/dynamic draws
            
        private:
            
            void clearDrawCall(DrawCall &t_call);
            void resetDrawCall(DrawCall &t_call);
            void prepareDrawCall(DrawCall &t_call);
            void initDrawCall(DrawCall &t_call, int t_primativeType, vertex::VertexLayout *t_vertexLayout, std::string t_shaderId, std::string t_key);
            
        private:
            
            std::vector<DrawCall> m_drawCalls;
            ShaderManager *m_shaderManager;
            
        };
    }
}

#endif