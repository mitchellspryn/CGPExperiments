#include "../include/RandomNumberGenerator.hpp"

#include <algorithm>

namespace cc = cgpExperiments::core;


cc::RandomNumberGenerator::RandomNumberGenerator(int seed) {
    generator_.seed(seed);
    floatDistribution_ = std::uniform_real_distribution<float>(0, 1);
}

float cc::RandomNumberGenerator::getRandomFloat() {
    float f = floatDistribution_(generator_);
    return f;
}

int cc::RandomNumberGenerator::getRandomInt(int lowerBoundInclusive, int upperBoundInclusive) {
    if (lowerBoundInclusive == upperBoundInclusive) {
        return lowerBoundInclusive;
    }

    float f = floatDistribution_(generator_);

    int range = upperBoundInclusive - lowerBoundInclusive + 1;
    int randomResult = (static_cast<int>(f * (range))) + lowerBoundInclusive;

    return randomResult;
}
