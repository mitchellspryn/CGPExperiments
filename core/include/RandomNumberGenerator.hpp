#pragma once

#include <random>

namespace cgpExperiments {
namespace core {
namespace randomNumberGenerator {
    void seedRng(int seed);
    float getRandomFloat();
    int getRandomInt(int lowerBoundInclusive, int upperBoundInclusive);
}
}
}
