#include "../include/RandomNumberGenerator.hpp"

namespace cc = cgpExperiments::core;

cc::RandomNumberGenerator::RandomNumberGenerator(int seed) {
    generator.seed(seed);
}

float cc::RandomNumberGenerator::getRandomFloat() {
    return distribution_(generator_);
}
