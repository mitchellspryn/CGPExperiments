#pragma once

namespace cgpExperiments {
namespace core {
class RandomNumberGenerator {
    public:
        explicit RandomNumberGenerator(int seed);
        float getRandomFloat();
};
}
}
