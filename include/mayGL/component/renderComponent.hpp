#ifndef renderComponent_hpp
#define renderComponent_hpp

#include "component.hpp"
#include <mayGL/camera/camera.hpp>
#include <mayGL/projection/baseProjection.hpp>
#include <mayGL/core/timer.hpp>

namespace mayGL
{
    namespace component
    {
        struct DrawCall 
        {
            unsigned int m_VAO, m_VBO, m_IBO;

            void *m_vertices;
            void *m_indices;

            unsigned int m_vertexDataOffset;
            unsigned int m_indicesDataOffset;
            unsigned int m_indicesValueOffset;
            int m_objects;

            std::map<int, int> m_textureUnits;
            std::vector<component::Texture*> m_textures;
        };

        struct uboMatrices
        {
            glm::mat4 m_Proj = glm::mat4(1);
            glm::mat4 m_View = glm::mat4(1);
            glm::vec4 m_CamPos = glm::vec4(0);
            float m_Time = 0;
        }; // 148 bytes

        class RenderComponent : public Component
        {
        public: // public static methods

            static void setCamera(renderer::Camera *t_camera);
            static void setProjection(renderer::BaseProjection *t_proj);

            static bool hasCamera();
            static bool hasProjection();

            static void preDraw();

        private: // private static attributes

            static renderer::BaseProjection *s_proj;
            static renderer::Camera *s_camera;

            static unsigned int s_matraciesUbo;
            static uboMatrices s_matricesData;
            static bool s_uboCreated;
            static float s_time;

        public: // public methods

            RenderComponent(entity::Entity* t_parent, std::string t_id, vertex::VertexLayout *t_layout);
            ~RenderComponent();

            vertex::VertexLayout* getLayout();
            void setShaders(renderer::Shader *t_shader);
            void loadShaders(std::string t_vert, std::string t_frag);
            void loadShaders(std::string t_vert, std::string t_geom, std::string t_frag);
            void loadShadersSource(std::string t_vert, std::string t_frag);
            void loadShadersSource(std::string t_vert, std::string t_geom, std::string t_frag);

            renderer::Shader *getShader();
            std::string getShaderId();

            void setPrimativeType(GLenum t_type);
            GLenum getPrimativeType();

            inline std::vector<Mesh*> getMeshes() { return m_meshes; }
            void addMesh(std::string t_meshId);
            void addMeshes(std::vector<std::string> t_meshIds);
            void addAllMeshes();
            void removeMesh(std::string t_meshId);
            void removeAllMeshes();

            // openGL face culling
            inline void enableFaceCulling() { m_cullFace = true; }
            inline void disableFaceCulling() { m_cullFace = false; }
            inline void faceCull(int t_face) { glCullFace(t_face); }

            // active
            inline bool isActive() { return m_active; }
            inline void active(bool t_active) { m_active = t_active; }

            // wireframe for batch
            inline void enableWireframe() { m_batchWireframe = true; }
            inline void disableWireframe() { m_batchWireframe = false; }

            inline void update() override {}
            void draw();

            void imguiComponentInspector() override;

        private:

        void push(Mesh *t_mesh);
        void clearBuffer();

        private: // private attributes

            GLenum m_primativeType;
            std::string m_shader;
            vertex::VertexLayout *m_layout;
            std::vector<Mesh*> m_meshes;
            float m_timePerDraw;
            float m_timePerPush;
            bool m_batchWireframe;
            bool m_cullFace;
            bool m_active;

            DrawCall m_drawCall;
        };
    }
}

#endif