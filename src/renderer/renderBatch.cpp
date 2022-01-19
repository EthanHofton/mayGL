#include <mayGL/renderer/renderBatch.hpp>

namespace mayGL
{
    namespace renderer
    {
        RenderBatch::RenderBatch()
        {
            m_shaderManager = ShaderManager::instance();
        }
        
        RenderBatch::~RenderBatch()
        {
            m_shaderManager = nullptr;
            
            for (auto &drawCall : m_drawCalls)
            {
                clearDrawCall(drawCall);
            }
        }
        
        void RenderBatch::clearDrawCall(DrawCall &t_call)
        {
            free(t_call.m_vertexArray);
            free(t_call.m_indexArray);
            
            glDeleteBuffers(1, &t_call.m_VBO);
            glDeleteBuffers(1, &t_call.m_IBO);
            glDeleteBuffers(1, &t_call.m_VAO);
        }
        
        void RenderBatch::push(entity::Entity *t_e)
        {
            if (!t_e->hasComponent(component::mesh))
            {
                return;
            }

            for (auto mesh : t_e->getComponents<component::Mesh, component::mesh>())
            {
                if (!mesh->isVisable())
                {
                    continue;
                }

                if (!mesh->hasShader())
                {
                    continue;
                }
                
                auto vertexLayout = mesh->getLayout();
                auto primativeType = mesh->getPrimativeType();
                auto shader = mesh->getShaderId();
                
                std::string key = std::to_string(primativeType) + shader;
                
                DrawCall *currentCall = nullptr;
                for (auto &call : m_drawCalls)
                {
                    if (key == call.m_drawCallKey && call.m_vertexLayout->getVertexStride() == vertexLayout->getVertexStride())
                    {
                        if (!call.m_full)
                        {
                            currentCall = &call;
                            break;
                        }
                    }
                }
                
                if (currentCall == nullptr)
                {
                    DrawCall call;
                    initDrawCall(call, primativeType, vertexLayout, shader, key);
                    m_drawCalls.push_back(call);
                    currentCall = &m_drawCalls.back();
                }
                
                // TODO: buffer overflow
                
                // TODO: textures
                if (mesh->hasTexture())
                {
                    component::Texture *tex = mesh->getTexture();
                    
                    if (currentCall->m_boundTextures.find(tex->getFileName()) == currentCall->m_boundTextures.end())
                    {
                        currentCall->m_boundTextures[tex->getFileName()] = (int)currentCall->m_boundTextures.size();
                        tex->setTextureUnit(currentCall->m_boundTextures[tex->getFileName()]);
                        currentCall->m_textures.push_back(tex);
                    } else {
                        tex->setTextureUnit(currentCall->m_boundTextures[tex->getFileName()]);
                    }
                    tex->updateTextureUnit();
                }
                
                void *indices = mesh->getIndices();
                void *vertices = mesh->getWorldVertices();
                
                // update indices
                unsigned int* indicesCopy = (unsigned int*)malloc(mesh->getIndicesSize());
                memcpy(indicesCopy, indices, mesh->getIndicesSize());
                int indexOffset = *std::max_element(indicesCopy, indicesCopy +  (mesh->getIndicesSize() / sizeof(unsigned int))) + 1;
                for (int i = 0; i < (mesh->getIndicesSize() / sizeof(unsigned int)); i++)
                {
                    indicesCopy[i] += currentCall->m_indexValueOffset;
                }

                memcpy((char *)currentCall->m_vertexArray + currentCall->m_vertexDataOffset, vertices, mesh->getVerticesSize());
                memcpy((char *)currentCall->m_indexArray + currentCall->m_indexDataOffset, indicesCopy, mesh->getIndicesSize());
                
                free(indicesCopy);

                currentCall->m_objects++;
                currentCall->m_indexValueOffset += indexOffset;
                currentCall->m_indexDataOffset += mesh->getIndicesSize();
                currentCall->m_vertexDataOffset += mesh->getVerticesSize();
            }
        }
        
        void RenderBatch::initDrawCall(DrawCall &t_call, int t_primativeType, vertex::VertexLayout *t_vertexLayout, std::string t_shaderId, std::string t_key)
        {
            t_call.m_primativeType = t_primativeType;
            t_call.m_vertexLayout = t_vertexLayout;
            t_call.m_shaderId = t_shaderId;
            t_call.m_drawCallKey = t_key;
            t_call.m_full = false;
            t_call.m_vertexDataOffset = 0;
            t_call.m_indexDataOffset = 0;
            t_call.m_indexValueOffset = 0;
            t_call.m_objects = 0;
            
            // gen vertices
            int maxNumVertices = GAME_CONFIG()["max_num_vertices"].GetInt();
            glGenVertexArrays(1, &t_call.m_VAO);
            glBindVertexArray(t_call.m_VAO);
            
            glGenBuffers(1, &t_call.m_IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_call.m_IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxNumVertices*sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
            
            glGenBuffers(1, &t_call.m_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, t_call.m_VBO);
            glBufferData(GL_ARRAY_BUFFER, maxNumVertices*t_call.m_vertexLayout->getVertexStride(), nullptr, GL_DYNAMIC_DRAW);
            
            t_call.m_vertexLayout->init();
            
            t_call.m_vertexArray = calloc(maxNumVertices, t_call.m_vertexLayout->getVertexStride());
            t_call.m_indexArray = calloc(maxNumVertices, sizeof(unsigned int));
        }
        
        void RenderBatch::batchBegin()
        {
            for (auto &call : m_drawCalls)
            {
                resetDrawCall(call);
            }
        }
        
        void RenderBatch::resetDrawCall(DrawCall &t_call)
        {
            int maxNumVertices = GAME_CONFIG()["max_num_vertices"].GetInt();
            memset(t_call.m_vertexArray, 0, maxNumVertices * t_call.m_vertexLayout->getVertexStride());
            memset(t_call.m_indexArray, 0, maxNumVertices * sizeof(unsigned int));
            
            t_call.m_indexDataOffset = 0;
            t_call.m_indexValueOffset = 0;
            t_call.m_vertexDataOffset = 0;
            t_call.m_full = false;
            t_call.m_objects = 0;
            t_call.m_boundTextures.clear();
            t_call.m_textures.clear();
        }
        
        void RenderBatch::batchEnd()
        {
            for (auto &call : m_drawCalls)
            {
                prepareDrawCall(call);
            }
        }
        
        void RenderBatch::prepareDrawCall(DrawCall &t_call)
        {
            if (t_call.m_objects == 0)
                return;
            
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, t_call.m_IBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, t_call.m_indexDataOffset, t_call.m_indexArray);
            
            glBindBuffer(GL_ARRAY_BUFFER, t_call.m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, t_call.m_vertexDataOffset, t_call.m_vertexArray);
        }

        void RenderBatch::draw(glm::mat4 &t_view, glm::mat4 &t_projection, glm::vec3 t_camPos, float t_time)
        {
            for (auto &call : m_drawCalls)
            {
                auto shader = m_shaderManager->getShader(call.m_shaderId);
                shader->useShader();
                
                // -- bind textures
                int samplers[16];
                for (int i = 0; i < 16; i++)
                {
                    samplers[i] = i;
                }
                glUniform1iv(shader->getUniformLocation("u_TexData"), 16, samplers);
                // -- bind textures
                
                for (auto tex : call.m_textures)
                {
                    tex->use();
                }
                
                
                glUniformMatrix4fv(shader->getUniformLocation("u_Proj"), 1, GL_FALSE, &t_projection[0][0]);
                glUniformMatrix4fv(shader->getUniformLocation("u_View"), 1, GL_FALSE, &t_view[0][0]);
                glUniform3f(shader->getUniformLocation("u_CamPos"), t_camPos.x, t_camPos.y, t_camPos.z);
                glUniform1f(shader->getUniformLocation("u_Time"), t_time);
                
                glBindVertexArray(call.m_VAO);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, call.m_IBO);
                
                glDrawElements(call.m_primativeType, (int)call.m_indexDataOffset / sizeof(unsigned int), GL_UNSIGNED_INT, 0);
                
                // glBindVertexArray(0);
                // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            }
        }
    }
}
