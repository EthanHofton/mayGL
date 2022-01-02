#include <camera/camera3D.hpp>

namespace may
{
    namespace renderer
    {
        Camera3D::Camera3D()
        {
            m_update = true;
            m_viewMatrix = glm::mat4(1.0f);
            m_wordUp = math::c_positiveY;
            m_yaw = 90.0f;
            m_pitch = 0.0f;
            m_front = glm::vec3(0.0f, 0.0f, -1.0f);
            m_moveSpeed = 10.0f;
            m_turnSpeed = 5.0f;
            
            getTransform()->pos(glm::vec3(0.0f));
            
            update();
        }
        
        void Camera3D::yawUpdate()
        {
            m_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front.y = sin(glm::radians(m_pitch));
            m_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
            m_front = glm::normalize(m_front);
            
            m_right = glm::normalize(glm::cross(m_front, m_wordUp));
            m_up = glm::normalize(glm::cross(m_right, m_front));
        }
        
        void Camera3D::update()
        {
            mouseControl();
            keyControl();
            
            if (m_update || getTransform()->needsTransformUpdate())
            {
                yawUpdate();
                m_viewMatrix = glm::lookAt(getTransform()->getPos(), getTransform()->getPos() + m_front, m_up);
                m_update = false;
            }
        }
        
        void Camera3D::keyControl()
        {
            if (core::InputManager::instance()->keyDown(SDL_SCANCODE_W))
            {
                glm::vec3 trans(0.0f);
                trans.z += m_front.z * m_moveSpeed * core::Timer::instance()->getDeltaTime();
                trans.x += m_front.x * m_moveSpeed * core::Timer::instance()->getDeltaTime();
                getTransform()->translate(trans);
            }
            
            if (core::InputManager::instance()->keyDown(SDL_SCANCODE_S))
            {
                glm::vec3 trans(0.0f);
                trans.z -= m_front.z * m_moveSpeed * core::Timer::instance()->getDeltaTime();
                trans.x -= m_front.x * m_moveSpeed * core::Timer::instance()->getDeltaTime();
                getTransform()->translate(trans);
            }
            
            if (core::InputManager::instance()->keyDown(SDL_SCANCODE_A))
            {
                glm::vec3 trans(0.0f);
                trans -= m_right * m_moveSpeed * (float)core::Timer::instance()->getDeltaTime();
                getTransform()->translate(trans);
            }
            
            if (core::InputManager::instance()->keyDown(SDL_SCANCODE_D))
            {
                glm::vec3 trans(0.0f);
                trans += m_right * m_moveSpeed * (float)core::Timer::instance()->getDeltaTime();
                getTransform()->translate(trans);
            }
            
            if (core::InputManager::instance()->keyDown(SDL_SCANCODE_SPACE))
            {
                glm::vec3 trans(0.0f);
                trans.y += m_moveSpeed * (float)core::Timer::instance()->getDeltaTime();
                getTransform()->translate(trans);
            }
            
            if (core::InputManager::instance()->keyDown(SDL_SCANCODE_LSHIFT))
            {
                glm::vec3 trans(0.0f);
                trans.y -= m_moveSpeed * (float)core::Timer::instance()->getDeltaTime();
                getTransform()->translate(trans);
            }
        }
        
        void Camera3D::mouseControl()
        {
            if (core::InputManager::instance()->mouseMotion())
            {
                glm::vec2 mouseRel = core::InputManager::instance()->getDeltaMousePos();
                core::InputManager::instance()->mouseMotion(false);
                mouseRel.x *= m_turnSpeed * core::Timer::instance()->getDeltaTime();
                mouseRel.y *= m_turnSpeed * core::Timer::instance()->getDeltaTime();
                
                m_yaw += mouseRel.x;
                m_pitch += mouseRel.y;
                
                if (m_pitch > 89.0f)
                {
                    m_pitch = 89.0;
                }
                
                if (m_pitch < -89.0f)
                {
                    m_pitch = -89.0;
                }
                
                m_update = true;
            }
        }
    }
}
