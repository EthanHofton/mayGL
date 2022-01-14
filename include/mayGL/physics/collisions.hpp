#ifndef collisions_hpp
#define collisions_hpp

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include "sphereCollider.hpp"
#include "cubeCollider.hpp"
#include "plainCollider.hpp"
#include "collisionManifold.hpp"

namespace mayGL
{
    namespace physics
    {
        bool sphereVsCube(SphereCollider *t_1, CubeCollider *t_2, Collision &t_c);
        bool cubeVsSphere(CubeCollider *t_1, SphereCollider *t_2, Collision &t_c);
        bool sphereVsSphere(SphereCollider *t_1, SphereCollider *t_2, Collision &t_c);
        bool cubeVsCube(CubeCollider *t_1, CubeCollider *t_2, Collision &t_c);
        bool plainVsPlain(PlainCollider *t_1, PlainCollider *t_2, Collision &t_c);
        bool cubeVsPlain(CubeCollider *t_1, PlainCollider *t_2, Collision &t_c);
        bool plainVsCube(PlainCollider *t_1, CubeCollider *t_2, Collision &t_c);
        bool plainVsSphere(PlainCollider *t_1, SphereCollider *t_2, Collision &t_c);
        bool sphereVsPlain(SphereCollider *t_1, PlainCollider *t_2, Collision &t_c);
    }
}

#endif