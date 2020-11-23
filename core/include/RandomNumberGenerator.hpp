#pragma once

#include <random>

namespace cgpExperiments {
namespace core {
class RandomNumberGenerator {
    public:
        explicit RandomNumberGenerator(int seed);
        float getRandomFloat();

    private:
        std::default_random_engine generator_;
        std::uniform_real_distribution<float> distribution_(0.0, 1.0);
};
}
}
