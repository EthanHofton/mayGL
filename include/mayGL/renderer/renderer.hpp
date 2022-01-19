#ifndef renderer_hpp
#define renderer_hpp

#include "renderBatch.hpp"
#include <mayGL/core/timer.hpp>

namespace mayGL
{
    namespace renderer
    {
        class Renderer
        {
        public:
            
            static Renderer* instance();
            static void release();
            
            void draw();
            
            void setProjection(BaseProjection *t_projection);
            void setCamera(Camera *t_cam);
            inline void push(RenderBatch *t_batch) { m_renderBatchQueue.push(t_batch); }
            
        private:
            
            Renderer();
            ~Renderer();
            
        private:
            
            bool m_hasCamera;
            bool m_hasProjection;
            
            Camera *m_camera;
            BaseProjection *m_projection;
            
            std::queue<RenderBatch*> m_renderBatchQueue;
            
            unsigned int m_vao;
            
            ShaderManager *m_shaderManager;
            
            static Renderer* s_instance;

            float m_time;
        };
    }
}


#endif