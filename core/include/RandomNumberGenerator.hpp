#pragma once

#include <random>

namespace cgpExperiments {
namespace core {

// A random number generator wrapper.
// A single instance should be created and used in all classes requiring RNG.
//
// This will allow determinism if single-threading is used.
class RandomNumberGenerator {
    public:
        RandomNumberGenerator(int seed);

        void seed(int seed);
        float getRandomFloat();
        int getRandomInt(int lowerBoundInclusive, int upperBoundInclusive);

    private:
        std::default_random_engine generator_;
        std::uniform_real_distribution<float> floatDistribution_;
};

}
}
