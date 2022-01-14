#include <mayGL/support/geometry.hpp>

namespace mayGL
{
    namespace geometry
    {
        glm::vec3 calculatePlainNormal(plain t_p)
        {
            glm::vec3 b = t_p.m_a;
            glm::vec3 r = t_p.m_b;
            glm::vec3 s = t_p.m_d;

            glm::vec3 n = glm::cross((r-b), (s-b));
            n = glm::normalize(n);
            return n;
        }
    }
}