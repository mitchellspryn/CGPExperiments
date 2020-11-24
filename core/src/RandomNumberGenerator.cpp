#include "../include/RandomNumberGenerator.hpp"

#include <algorithm>

namespace cc = cgpExperiments::core;

cc::RandomNumberGenerator::RandomNumberGenerator(int seed) {
    generator.seed(seed);
}

float cc::RandomNumberGenerator::getRandomFloat() {
    return distribution_(generator_);
}

int cc::RandomNumberGenerator::getRandomInt(int lowerBoundInclusive, int upperBoundInclusive) {
    float f = distribution_(generator_);

    int range = upperBoundInclusive - lowerBoundInclusive;
    
    return ((static_cast<int>(f * (range))) % range) + lowerBoundInclusive;

}
