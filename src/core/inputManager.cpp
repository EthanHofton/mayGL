#include <mayGL/core/inputManager.hpp>

namespace mayGL
{
    namespace core
    {
        InputManager *InputManager::s_instance = nullptr;
        
        InputManager *InputManager::instance()
        {
            if (s_instance == nullptr)
            {
                s_instance = new InputManager;
            }
            
            return s_instance;
        }
        
        void InputManager::release()
        {
            if (s_instance != nullptr)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }
        
        InputManager::InputManager()
        {
            m_mouseFirst = true;
            m_mouseMovement = false;
            
            m_keyboardStates = SDL_GetKeyboardState(&m_keyLeangh);
            m_prevKeyState = new Uint8[m_keyLeangh];
            memcpy(m_prevKeyState, m_keyboardStates, m_keyLeangh);
            
            CORE_INFO("input manager initalised");
        }
        
        InputManager::~InputManager()
        {
            CORE_INFO("input manager deleted");
        }
        
        bool InputManager::mouseButtonDown(MOUSE_BUTTONS t_button)
        {
            Uint32 mask = 0;
            
            switch (t_button) {
                case left:
                    mask = SDL_BUTTON(SDL_BUTTON_LEFT);
                    break;
                    
                case right:
                    mask = SDL_BUTTON(SDL_BUTTON_RIGHT);
                    break;
                    
                case middle:
                    mask = SDL_BUTTON(SDL_BUTTON_MIDDLE);
                    break;
                    
                case back:
                    mask = SDL_BUTTON(SDL_BUTTON_X1);
                    break;
                    
                case forward:
                    mask = SDL_BUTTON(SDL_BUTTON_X2);
                    break;
            }
            
            return (m_mouseState & mask) && !(ImGui::GetIO().WantCaptureMouse);
        }
        
        bool InputManager::mouseButtonPressed(MOUSE_BUTTONS t_button)
        {
            Uint32 mask = 0;
            
            switch (t_button) {
                case left:
                    mask = SDL_BUTTON(SDL_BUTTON_LEFT);
                    break;
                    
                case right:
                    mask = SDL_BUTTON(SDL_BUTTON_RIGHT);
                    break;
                    
                case middle:
                    mask = SDL_BUTTON(SDL_BUTTON_MIDDLE);
                    break;
                    
                case back:
                    mask = SDL_BUTTON(SDL_BUTTON_X1);
                    break;
                    
                case forward:
                    mask = SDL_BUTTON(SDL_BUTTON_X2);
                    break;
            }
            
            return !(m_prevMouseState & mask) && (m_mouseState & mask) && !(ImGui::GetIO().WantCaptureMouse);
        }
        
        bool InputManager::mouseButtonReleased(MOUSE_BUTTONS t_button)
        {
            Uint32 mask = 0;
            
            switch (t_button) {
                case left:
                    mask = SDL_BUTTON_LMASK;
                    break;
                    
                case right:
                    mask = SDL_BUTTON_RMASK;
                    break;
                    
                case middle:
                    mask = SDL_BUTTON_MMASK;
                    break;
                    
                case back:
                    mask = SDL_BUTTON_X1MASK;
                    break;
                    
                case forward:
                    mask = SDL_BUTTON_X2MASK;
                    break;
            }
            
            return (m_prevMouseState & mask) && !(m_mouseState & mask) && !(ImGui::GetIO().WantCaptureMouse);
        }
        
        void InputManager::giveEvents(SDL_Event t_e)
        {
            m_event = t_e;
            
            if (m_event.type == SDL_MOUSEMOTION)
            {
                m_mouseMovement = !(ImGui::GetIO().WantCaptureMouse);
                m_dmPos.x = m_event.motion.xrel;
                m_dmPos.y = -(m_event.motion.yrel);
            }
        }
    }
}
