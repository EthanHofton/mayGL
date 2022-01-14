#ifndef collisionManifold_hpp
#define collisionManifold_hpp

#include <glm/glm.hpp>

namespace mayGL
{
    namespace physics
    {
        class Collider;
        
        struct Collision
        {
            Collider *m_c1;
            Collider *m_c2;

            glm::vec3 m_e1Contact;
            glm::vec3 m_e2Contact;
        };
    }
}

#endif