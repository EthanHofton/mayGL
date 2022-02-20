#include <mayGL/component/renderComponent.hpp>

namespace mayGL
{
    namespace component
    {
        renderer::Camera *RenderComponent::s_camera = nullptr;
        renderer::BaseProjection *RenderComponent::s_proj = nullptr;
        uboMatrices RenderComponent::s_matricesData;
        unsigned int RenderComponent::s_matraciesUbo;
        bool RenderComponent::s_uboCreated = false;
        float RenderComponent::s_time = 0;

        void RenderComponent::setCamera(renderer::Camera *t_camera)
        {
            if (!s_uboCreated)
            {
                glGenBuffers(1, &s_matraciesUbo);
                glBindBuffer(GL_UNIFORM_BUFFER, s_matraciesUbo);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(uboMatrices), NULL, GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_matraciesUbo);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
                s_uboCreated = true;
            }

            s_camera = t_camera;
        }

        void RenderComponent::setProjection(renderer::BaseProjection *t_proj)
        {
            if (!s_uboCreated)
            {
                glGenBuffers(1, &s_matraciesUbo);
                glBindBuffer(GL_UNIFORM_BUFFER, s_matraciesUbo);
                glBufferData(GL_UNIFORM_BUFFER, sizeof(uboMatrices), NULL, GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_matraciesUbo);
                glBindBuffer(GL_UNIFORM_BUFFER, 0);
                s_uboCreated = true;
            }

            s_proj = t_proj;
        }

        bool RenderComponent::hasCamera()
        {
            return (s_camera != nullptr);
        }

        bool RenderComponent::hasProjection()
        {
            return (s_proj != nullptr);
        }

        RenderComponent::RenderComponent(entity::Entity* t_parent, std::string t_id, vertex::VertexLayout *t_layout) : Component(t_parent, renderer, t_id)
        {
            m_shader = "";
            m_primativeType = GL_TRIANGLES;
            m_layout = t_layout;
            m_batchWireframe = false;
            m_cullFace = false;

            int maxNumVertices = GAME_CONFIG()["max_num_vertices"].GetInt();
            int maxNumIndices = GAME_CONFIG()["max_num_indices"].GetInt();

            m_drawCall = DrawCall();
            m_drawCall.m_indices = calloc(maxNumIndices, sizeof(unsigned int));
            m_drawCall.m_vertices = calloc(maxNumVertices, m_layout->getVertexStride());
            m_drawCall.m_vertexDataOffset = 0;
            m_drawCall.m_indicesDataOffset = 0;
            m_drawCall.m_indicesValueOffset = 0;
            m_drawCall.m_objects = 0;

            glGenVertexArrays(1, &m_drawCall.m_VAO);
            glBindVertexArray(m_drawCall.m_VAO);
            
            glGenBuffers(1, &m_drawCall.m_IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawCall.m_IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, maxNumIndices*sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);
            
            glGenBuffers(1, &m_drawCall.m_VBO);
            glBindBuffer(GL_ARRAY_BUFFER, m_drawCall.m_VBO);
            glBufferData(GL_ARRAY_BUFFER, maxNumVertices*m_layout->getVertexStride(), nullptr, GL_DYNAMIC_DRAW);

            m_layout->init();
        }

        RenderComponent::~RenderComponent()
        {
            m_layout = nullptr;

            free(m_drawCall.m_indices);
            free(m_drawCall.m_vertices);

            glDeleteBuffers(1, &m_drawCall.m_VAO);
            glDeleteBuffers(1, &m_drawCall.m_IBO);
            glDeleteBuffers(1, &m_drawCall.m_VBO);
        }

        vertex::VertexLayout* RenderComponent::getLayout()
        {
            return m_layout;
        }

        void RenderComponent::setShaders(renderer::Shader *t_shader)
        {
            m_shader = renderer::ShaderManager::instance()->shaderID(t_shader);
        }

        void RenderComponent::loadShaders(std::string t_vert, std::string t_frag)
        {
            m_shader = renderer::ShaderManager::instance()->loadFromFile(t_vert, t_frag);
        }

        void RenderComponent::loadShaders(std::string t_vert, std::string t_geom, std::string t_frag)
        {
            m_shader = renderer::ShaderManager::instance()->loadFromFile(t_vert, t_geom, t_frag);
        }

        void RenderComponent::loadShadersSource(std::string t_vert, std::string t_frag)
        {
            m_shader = renderer::ShaderManager::instance()->loadFromText(t_vert, t_frag);
        }

        void RenderComponent::loadShadersSource(std::string t_vert, std::string t_geom, std::string t_frag)
        {
            m_shader = renderer::ShaderManager::instance()->loadFromText(t_vert, t_geom, t_frag);
        }

        renderer::Shader *RenderComponent::getShader()
        {
            return renderer::ShaderManager::instance()->getShader(m_shader);
        }

        std::string RenderComponent::getShaderId()
        {
            return m_shader;
        }

        void RenderComponent::setPrimativeType(GLenum t_type)
        {
            m_primativeType = t_type;
        }

        GLenum RenderComponent::getPrimativeType()
        {
            return m_primativeType;
        }

        void RenderComponent::push(Mesh* t_mesh)
        {
            if (!t_mesh->isVisable())
            {
                return;
            }

            // catch buffer overflow
            // if buffer overflow occurs, draw and clear buffer


            void *indices = t_mesh->getIndices();
            void *vertices = t_mesh->getWorldVertices();

            // update indices
            unsigned int* indicesCopy = (unsigned int*)malloc(t_mesh->getIndicesSize());
            memcpy(indicesCopy, indices, t_mesh->getIndicesSize());
            int indexOffset = *std::max_element(indicesCopy, indicesCopy +  (t_mesh->getIndicesSize() / sizeof(unsigned int))) + 1;
            for (int i = 0; i < (t_mesh->getIndicesSize() / sizeof(unsigned int)); i++)
            {
                indicesCopy[i] += m_drawCall.m_indicesValueOffset;
            }

            // copy mesh vertice and indice data into draw call buffer
            memcpy((char *)m_drawCall.m_vertices + m_drawCall.m_vertexDataOffset, vertices, t_mesh->getVerticesSize());
            memcpy((char *)m_drawCall.m_indices + m_drawCall.m_indicesDataOffset, indicesCopy, t_mesh->getIndicesSize());

            // delete the indices copy
            free(indicesCopy);

            // update draw call variables
            m_drawCall.m_objects++;
            m_drawCall.m_indicesValueOffset += indexOffset;
            m_drawCall.m_indicesDataOffset += t_mesh->getIndicesSize();
            m_drawCall.m_vertexDataOffset += t_mesh->getVerticesSize();
        }

        void RenderComponent::clearBuffer()
        {
            // write the data from the draw call into the draw buffers
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawCall.m_IBO);
            glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_drawCall.m_indicesDataOffset, m_drawCall.m_indices);
            
            glBindBuffer(GL_ARRAY_BUFFER, m_drawCall.m_VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, m_drawCall.m_vertexDataOffset, m_drawCall.m_vertices);

            // use the shader
            auto shader = renderer::ShaderManager::instance()->getShader(m_shader);
            shader->useShader();

            unsigned int matBlockIndex = glGetUniformBlockIndex(shader->getShaderId(), "Matrices");
            glUniformBlockBinding(shader->getShaderId(), matBlockIndex, 0);

            glBindVertexArray(m_drawCall.m_VAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_drawCall.m_IBO);

            if (m_batchWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }

            if (m_cullFace)
            {
                glEnable(GL_CULL_FACE);
            }
            
            glDrawElements(m_primativeType, (int)m_drawCall.m_indicesDataOffset / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

            if (m_batchWireframe)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            
            if (m_cullFace)
            {
                glDisable(GL_CULL_FACE);
            }

            // clear draw call vertex and index array
            // bzero(m_drawCall.m_indices, m_drawCall.m_indicesDataOffset);
            // bzero(m_drawCall.m_vertices, m_drawCall.m_vertexDataOffset);
            int maxNumVertices = GAME_CONFIG()["max_num_vertices"].GetInt();
            int maxNumIndices = GAME_CONFIG()["max_num_indices"].GetInt();
            memset(m_drawCall.m_vertices, 0, maxNumVertices*m_layout->getVertexStride());
            memset(m_drawCall.m_indices, 0, maxNumIndices*sizeof(unsigned int));

            // reset array sizes
            m_drawCall.m_indicesDataOffset = 0;
            m_drawCall.m_indicesValueOffset = 0;
            m_drawCall.m_vertexDataOffset = 0;
            m_drawCall.m_objects++;
        }

        void RenderComponent::preDraw()
        {
            if (s_uboCreated)
            {
                glm::mat4 projectionMatrix = glm::mat4(1.0f);
                glm::mat4 viewMatrix = glm::mat4(1.0f);
                glm::vec3 camPos = glm::vec3(0.0f);
                s_time += core::Timer::instance()->getDeltaTime();
                
                if (hasCamera())
                {
                    viewMatrix = s_camera->getViewMatrix();
                    camPos = s_camera->getTransform()->getPos();
                }
                
                if (hasProjection())
                {
                    projectionMatrix = s_proj->getProjectionMatrix();
                }

                // ubo bollox
                s_matricesData.m_Proj = projectionMatrix;
                s_matricesData.m_View = viewMatrix;
                s_matricesData.m_Time = s_time;
                s_matricesData.m_CamPos = glm::vec4(camPos, 0);

                glBindBuffer(GL_UNIFORM_BUFFER, s_matraciesUbo);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(s_matricesData), (void*)&s_matricesData);
            }
        }

        void RenderComponent::draw()
        {
            // add all meshes to draw call
            for (auto mesh : m_meshes)
            {
                push(mesh);
            }

            // clear the draw call buffer by rendering buffer to screen
            clearBuffer();
        }

        void RenderComponent::addMesh(std::string t_meshId)
        {
            m_meshes.push_back(getParent()->getComponent<Mesh, component::mesh>(t_meshId));
            CORE_INFO("RenderComponent with id '{}' added mesh with id '{}'", m_id, t_meshId);
        }

        void RenderComponent::addMeshes(std::vector<std::string> t_meshIds)
        {
            for (auto meshId : t_meshIds)
            {
                addMesh(meshId);
            }

            CORE_INFO("RenderComponent with id '{}' added {} meshes", m_id, t_meshIds.size());
        }

        void RenderComponent::addAllMeshes()
        {
            m_meshes.clear();
            m_meshes = getParent()->getComponents<Mesh, component::mesh>();

            CORE_INFO("RenderComponent with id '{}' added all meshes", m_id);
        }

        void RenderComponent::removeMesh(std::string t_meshId)
        {
            int index = 0;
            bool found = false;
            for (; index < m_meshes.size(); index++)
            {
                if (m_meshes[index]->getId() == t_meshId)
                {
                    found = true;
                    break;
                }
            }
            
            if (found)
            {
                CORE_INFO("RenderComponent with id '{}' removed mesh with id '{}'", m_id, t_meshId);
                m_meshes.erase(m_meshes.begin() + index);
            }
        }

        void RenderComponent::removeAllMeshes()
        {
            CORE_INFO("RenderComponent with id '{}' removed all bound meshes", m_id);
            m_meshes.clear();
        }
    }
}