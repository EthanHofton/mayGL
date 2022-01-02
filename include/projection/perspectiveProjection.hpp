#ifndef perspectiveProjection_hpp
#define perspectiveProjection_hpp

#include "baseProjection.hpp"

namespace may
{
    namespace renderer
    {
        class PerspectiveProjection : public BaseProjection
        {
        public:
            
            PerspectiveProjection();
            ~PerspectiveProjection();
        };
    }
}

#endif