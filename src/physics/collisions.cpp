#include <mayGL/physics/collisions.hpp>
#include <mayGL/entity/entity.hpp>

namespace mayGL
{
    namespace physics
    {
        bool sphereVsCube(SphereCollider *t_1, CubeCollider *t_2, Collision &t_c)
        {
            return cubeVsSphere(t_2, t_1, t_c);
        }

        bool cubeVsSphere(CubeCollider *t_1, SphereCollider *t_2, Collision &t_c)
        {
            glm::vec3 spherePos = t_2->getTransform()->getPos();
            glm::vec3 closestPointOnCube = t_1->getClosesPointToCube(spherePos);
            
            float radiusSqrd = t_2->getRadius() * t_2->getRadius();
            float distSqrdToCube = glm::distance2(closestPointOnCube, spherePos);
            
            if (distSqrdToCube > radiusSqrd)
            {
                return false;
            }

            t_c.m_c1 = t_1;
            t_c.m_c2 = t_2;
            t_c.m_e1Contact = t_2->closesPointToSphere(closestPointOnCube);
            t_c.m_e2Contact = closestPointOnCube;

            return true;
        }

        bool sphereVsSphere(SphereCollider *t_1, SphereCollider *t_2, Collision &t_c)
        {
            glm::vec3 t1Pos = t_1->getTransform()->getPos();
            glm::vec3 t2Pos = t_2->getTransform()->getPos();

            float dist = glm::distance2(t2Pos, t1Pos);
            float radiusSquered = (t_1->getRadius() + t_2->getRadius()) * (t_1->getRadius() + t_2->getRadius());
            if (dist > radiusSquered)
            {
                return false;
            }

            t_c.m_c1 = t_1;
            t_c.m_c2 = t_2;
            t_c.m_e1Contact = t_2->closesPointToSphere(t1Pos);
            t_c.m_e2Contact = t_1->closesPointToSphere(t2Pos);

            return true;
        }

        bool cubeVsCube(CubeCollider *t_1, CubeCollider *t_2, Collision &t_c)
        {
            return false;
        }

        bool plainVsPlain(PlainCollider *t_1, PlainCollider *t_2, Collision &t_c)
        {
            return false;
        }

        bool cubeVsPlain(CubeCollider *t_1, PlainCollider *t_2, Collision &t_c)
        {
            return plainVsCube(t_2, t_1, t_c);
        }

        bool plainVsCube(PlainCollider *t_1, CubeCollider *t_2, Collision &t_c)
        {
            return false;
        }

        bool plainVsSphere(PlainCollider *t_1, SphereCollider *t_2, Collision &t_c)
        {
            glm::vec3 spherePos = t_2->getTransform()->getPos();

            if (t_1->smallestDistToPoint(spherePos) > t_2->getRadius())
            {
                return false;
            }

            glm::vec3 posOnPlain = t_1->closestPointToPlain(spherePos);

            if (!t_1->pointInPlain(posOnPlain))
            {
                return false;
            }


            glm::vec3 posOnSphere = t_2->closesPointToSphere(posOnPlain);
            t_c.m_c1 = t_1;
            t_c.m_c2 = t_2;
            t_c.m_e2Contact = posOnPlain;
            t_c.m_e1Contact = posOnSphere;

            return true;
        }

        bool sphereVsPlain(SphereCollider *t_1, PlainCollider *t_2, Collision &t_c)
        {
            return plainVsSphere(t_2, t_1, t_c);
        }
    }
}