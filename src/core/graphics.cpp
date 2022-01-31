#include <mayGL/core/graphics.hpp>

namespace mayGL
{
    namespace core
    {
        // initalise static variables
        Graphics *Graphics::s_instance = nullptr;
        
        Graphics* Graphics::instance()
        {
            if (s_instance == nullptr)
            {
                s_instance = new Graphics;
            }
            
            return s_instance;
        }
        
        void Graphics::release()
        {
            if (s_instance != nullptr)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }
        
        Graphics::Graphics()
        {
            // initalise variables
            m_title = "MayGL Engine default title";
            m_color = {1.0f, 1.0f, 1.0f, 1.0f};
            m_wireframe = false;
            m_relativeMouse = false;
            m_pos = {SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED};
            
            // set initalised to false
            m_initalised = false;
        }
        
        Graphics::~Graphics()
        {
            // imgui cleanup
            CORE_INFO("Cleaning up imGui");
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplSDL2_Shutdown();

            deleteContext();
            destroyWindow();
            
            CORE_INFO("quitting SDL");
            SDL_Quit();
        }
        
        void Graphics::init()
        {
            // initalise SDL and check for errors
            if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
            {
                CORE_CRITICAL("SDL ERROR: failed to initalize SDL");
                return;
            }
            
            // log SDL initalised correctly
            CORE_INFO("SDL initalized");
            
            // set OpenGL attributes
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
            
            // create window instance
            int width = CONFIG()->getWidth();
            int height = CONFIG()->getHeight();
            m_window = SDL_CreateWindow(m_title.c_str(), m_pos.x, m_pos.y, width, height, SDL_WINDOW_OPENGL);
            
            // check if window was created succsufuly
            if (m_window == nullptr)
            {
                // log critical error that the window failed to be created
                CORE_CRITICAL("SDL ERROR: failed to create window");
                return;
            }
            
            // log window created succsufuly
            CORE_INFO("window created");
            
            // create OpenGL contex
            m_context = SDL_GL_CreateContext(m_window);
            
            // log context was created succsuefuly
            CORE_INFO("OpenGL context created");

            // Setup imGui
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            ImGui::StyleColorsDark();
            ImGui_ImplSDL2_InitForOpenGL(m_window, m_context);
            ImGui_ImplOpenGL3_Init("#version 400");
            CORE_INFO("ImGui initalised");
            
            // set glew experimental to true to use full features
            glewExperimental = GL_TRUE;
            
            // initalise glew
            if (glewInit() != GLEW_OK)
            {
                // log fatal error
                CORE_CRITICAL("GLEW ERROR: failed to initalsie flew");
                return;
            }

            SDL_GL_SetSwapInterval(0);
            
            // set up additional OpenGL params
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            glViewport(0, 0, width, height);
            glEnable(GL_PROGRAM_POINT_SIZE);
            
            glEnable(GL_PROGRAM_POINT_SIZE);
            
            relativeMouse(m_relativeMouse);
            wireframe(m_wireframe);          
            
            CORE_INFO("initalisation completed");
            
            m_initalised = true;
            return;
        }
        
    }
}
