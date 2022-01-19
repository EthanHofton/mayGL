#include <mayGL/renderer/renderer.hpp>

namespace mayGL
{
    namespace renderer
    {
        Renderer *Renderer::s_instance = nullptr;
        
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
        }
        
        Renderer::~Renderer()
        {
            glDeleteBuffers(1, &m_vao);
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
            
            while (!m_renderBatchQueue.empty())
            {
                auto batch = m_renderBatchQueue.front();
                batch->draw(viewMatrix, projectionMatrix, camPos, m_time);
                m_renderBatchQueue.pop();
            }
        }
    }
}
