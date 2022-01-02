#ifndef random_hpp
#define random_hpp

#include <iostream>
#include <stdio.h>
#include <time.h>
#include <random>
#include <unistd.h>

namespace may
{
    namespace random
    {
        class prng
        {
        public:

            prng(std::string t_seed = "");

            float randf(float t_lower = 0.0f, float t_upper = 1.0f);
            int randi(int t_lower = 0, int t_upper = 1);

        private:

            std::string m_seed;
            std::mt19937 m_generator;

        };
    }
}

#endif