#ifndef orthographicProjection_hpp
#define orthographicProjection_hpp

#include "baseProjection.hpp"

namespace may
{
    namespace renderer
    {
        class OrthographicProjection : public BaseProjection
        {
        public:
            
            OrthographicProjection();
            ~OrthographicProjection();
        };
    }
}

#endif