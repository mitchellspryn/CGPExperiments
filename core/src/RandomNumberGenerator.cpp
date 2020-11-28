#include "../include/RandomNumberGenerator.hpp"

#include <algorithm>

namespace ccr = cgpExperiments::core::randomNumberGenerator;

namespace {
    std::default_random_engine generator;
    std::uniform_real_distribution<float> floatDistribution(0, 1);
}

void ccr::seedRng(int seed) {
    generator.seed(seed);
}

float ccr::getRandomFloat() {
    return floatDistribution(generator);
}

int ccr::getRandomInt(int lowerBoundInclusive, int upperBoundInclusive) {
    if (lowerBoundInclusive == upperBoundInclusive) {
        return lowerBoundInclusive;
    }

    float f = floatDistribution(generator);

    int range = upperBoundInclusive - lowerBoundInclusive;

    return ((static_cast<int>(f * (range))) % range) + lowerBoundInclusive;
}
