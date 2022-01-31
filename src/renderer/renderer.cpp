#include <mayGL/renderer/renderer.hpp>

namespace mayGL
{
    namespace renderer
    {
        Renderer *Renderer::s_instance = nullptr;
        // unsigned int Renderer::s_textureUbo;
        
        Renderer *Renderer::instance()
        {
            if (s_instance == nullptr)
            {
                s_instance = new Renderer;
            }
            
            return s_instance;
        }
        
        void Renderer::release()
        {
            if (s_instance != nullptr)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }
        
        Renderer::Renderer()
        {
            m_shaderManager = ShaderManager::instance();
            
            m_hasCamera = false;
            m_hasProjection = false;
            
            glGenVertexArrays(1, &m_vao);
            glBindVertexArray(m_vao);

            m_time = 0;
            m_matracies = uboMatrices();
            m_textures = uboTextures();

            /// matrices ubo
            glGenBuffers(1, &m_matraciesUbo);
            glBindBuffer(GL_UNIFORM_BUFFER, m_matraciesUbo);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(uboMatrices), NULL, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_matraciesUbo);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            // texture ubo
            glGenBuffers(1, &m_textureUbo);
            glBindBuffer(GL_UNIFORM_BUFFER, m_textureUbo);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(uboTextures), NULL, GL_DYNAMIC_DRAW);
            glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_textureUbo);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            for (int i = 0; i < 32; i++)
            {
                m_textures.m_textureMaps[i] = glm::vec4(1);
            }
        }
        
        Renderer::~Renderer()
        {
            glDeleteBuffers(1, &m_vao);
            glDeleteBuffers(1, &m_matraciesUbo);
            glDeleteBuffers(1, &m_textureUbo);
        }
        
        void Renderer::setProjection(BaseProjection *t_projection)
        {
            CORE_INFO("renderer projection matrix set");
            m_hasProjection = true;
            m_projection = t_projection;
        }
        
        void Renderer::setCamera(Camera *t_cam)
        {
            CORE_INFO("renderer camera set");
            m_hasCamera = true;
            m_camera = t_cam;
        }
        
        void Renderer::draw()
        {
            glm::mat4 projectionMatrix = glm::mat4(1.0f);
            glm::mat4 viewMatrix = glm::mat4(1.0f);
            glm::vec3 camPos = glm::vec3(0.0f);
            m_time += core::Timer::instance()->getDeltaTime();
            
            if (m_hasCamera)
            {
                viewMatrix = m_camera->getViewMatrix();
                camPos = m_camera->getTransform()->getPos();
            }
            
            if (m_hasProjection)
            {
                projectionMatrix = m_projection->getProjectionMatrix();
            }

            // ubo bollox
            m_matracies.m_Proj = projectionMatrix;
            m_matracies.m_View = viewMatrix;
            m_matracies.m_Time = m_time;
            m_matracies.m_CamPos = glm::vec4(camPos, 0);

            glBindBuffer(GL_UNIFORM_BUFFER, m_matraciesUbo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(m_matracies), (void*)&m_matracies);

            glBindBuffer(GL_UNIFORM_BUFFER, m_textureUbo);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(m_textures), (void *)&m_textures);
            // ubo bollox
            
            while (!m_renderBatchQueue.empty())
            {
                auto batch = m_renderBatchQueue.front();
                batch->draw();
                m_renderBatchQueue.pop();
            }
        }
    }
}
