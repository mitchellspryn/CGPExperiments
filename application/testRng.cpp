#include <iostream>
#include <map>
#include <memory>
#include <string>


#include "../core/include/RandomNumberGenerator.hpp"

namespace cc = cgpExperiments::core;

static constexpr int kNumTests = 100000;
static constexpr int kRngSeed = 12345;

void printFloatTest() {
    std::map<int, int> counts;
    const int kNumBuckets = 10;
    float bucketStep = 1.0f / static_cast<float>(kNumBuckets);

    std::unique_ptr<cc::RandomNumberGenerator> rng =
        std::make_unique<cc::RandomNumberGenerator>(kRngSeed);

    for (int i = 0; i < kNumTests; i++) {
        float f = rng->getRandomFloat();

        for (int i = 0; i < kNumBuckets; i++) { 
            float fi = static_cast<float>(i);
            if ((fi * bucketStep) >= f) {
                counts[i]++;
                break;
            }
        }
    }

    std::cout << "float test: " << std::endl;
    for (const auto& kvp : counts) {
        std::cout 
            << "\t"
            << std::to_string(kvp.first)
            << ": "
            << std::to_string(kvp.second)
            << "\n";
    }
}

void printIntTest() {
    std::map<int, int> counts;
    constexpr int kUpperLimit = 10;

    std::unique_ptr<cc::RandomNumberGenerator> rng = 
        std::make_unique<cc::RandomNumberGenerator>(kRngSeed);

    for (int i = 0; i < kNumTests; i++) {
        int randomInt = rng->getRandomInt(0, kUpperLimit);
        counts[randomInt]++;
    }

    std::cout << "int test: " << std::endl;
    for (const auto& kvp : counts) {
        std::cout 
            << "\t"
            << std::to_string(kvp.first)
            << ": "
            << std::to_string(kvp.second)
            << "\n";
    }
}

int main(int argc, char** argv) {
    printFloatTest();
    printIntTest();

    return EXIT_SUCCESS;
}
