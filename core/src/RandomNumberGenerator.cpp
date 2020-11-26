#include "../include/RandomNumberGenerator.hpp"

#include <algorithm>

namespace ccr = cgpExperiments::core::randomNumberGenerator;

namespace {
    std::default_random_engine generator;
    std::uniform_real_distribution<float> floatDistribution(0, 1);
}

ccr::seedRng(int seed) {
    generator.seed(seed);
}

float ccr::getRandomFloat() {
    return distribution(generator);
}

int ccr::getRandomInt(int lowerBoundInclusive, int upperBoundInclusive) {
    float f = distribution(generator);

    int range = upperBoundInclusive - lowerBoundInclusive;
    
    return ((static_cast<int>(f * (range))) % range) + lowerBoundInclusive;
}
