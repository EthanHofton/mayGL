#ifndef camera3D_hpp
#define camera3D_hpp

#include "camera.hpp"
#include "../core/inputManager.hpp"
#include "../core/timer.hpp"

namespace may
{
    namespace renderer
    {
        class Camera3D : public Camera
        {
        public:
            
            Camera3D();
            inline ~Camera3D() {}
            
            virtual void keyControl();
            void mouseControl();
            
            inline glm::vec3 getDirection() { return m_front; }

            inline float getYaw() { return m_yaw; }
            inline void setYaw(float t_yaw) { m_yaw = t_yaw; }

            inline float getPitch() { return m_pitch; }
            inline void setPitch(float t_pitch) { m_pitch = t_pitch; }

            inline float getTurnSpeed() { return m_turnSpeed; }
            inline void setTurnSpeed(float t_turnSpeed) { m_turnSpeed = t_turnSpeed; }

            inline float getMoveSpeed() { return m_moveSpeed; }
            inline void setMoveSpeed(float t_moveSpeed) { m_moveSpeed = t_moveSpeed; }

            inline glm::vec3 getWorldUp() { return m_wordUp; }
            inline void setWorldUp(glm::vec3 t_worldUp) { m_wordUp = t_worldUp; }

            inline glm::vec3 getFront() { return m_front; }
            inline glm::vec3 getRight() { return m_right; }
            
            void update() override;
            inline glm::mat4 getViewMatrix() override { return m_viewMatrix; }
            
        private:
            
            void yawUpdate();
            
        private:
            
            glm::mat4 m_viewMatrix;
            
            glm::vec3 m_front;
            glm::vec3 m_up;
            glm::vec3 m_right;
            glm::vec3 m_wordUp;
            
            float m_yaw;
            float m_pitch;
            
            float m_moveSpeed;
            float m_turnSpeed;
            
            bool m_update;
        };
    }
}

#endif