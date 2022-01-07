#ifndef geometry_hpp
#define geometry_hpp

#include <glm/glm.hpp>

namespace may
{
    namespace geometry
    {
        struct plain
        {
            plain(glm::vec3 t_a, glm::vec3 t_b, glm::vec3 t_c, glm::vec3 t_d) : m_a(t_a), m_b(t_b), m_c(t_c), m_d(t_c) {}
            plain() {}

            glm::vec3 m_a;
            glm::vec3 m_b;
            glm::vec3 m_c;
            glm::vec3 m_d;
        };

        glm::vec3 calculatePlainNormal(plain t_p);
    }
}

#endif