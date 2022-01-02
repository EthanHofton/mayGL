#include <support/random.hpp>

namespace may
{
    namespace random
    {
        prng::prng(std::string t_seed)
        {
            std::random_device rd;
            m_generator = std::mt19937(rd());

            m_seed = (t_seed == "") ? std::to_string(getpid() * time(NULL)) : t_seed;

            std::seed_seq seed(m_seed.begin(), m_seed.end());

            m_generator.seed(seed);
        }

        float prng::randf(float t_lower, float t_upper)
        {
            std::uniform_real_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }

        int prng::randi(int t_lower, int t_upper)
        {
            std::uniform_int_distribution<> dist(t_lower, t_upper);
            return dist(m_generator);
        }
    }
}