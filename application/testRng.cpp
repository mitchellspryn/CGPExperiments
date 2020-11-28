#include <iostream>
#include <map>
#include <string>


#include "../core/include/RandomNumberGenerator.hpp"

namespace ccr = cgpExperiments::core::randomNumberGenerator;

static constexpr int kNumTests = 100000;

void printFloatTest() {
    std::map<int, int> counts;

    const int kNumBuckets = 10;
    float bucketStep = 1.0f / static_cast<float>(kNumBuckets);
    for (int i = 0; i < kNumTests; i++) {
        float f = ccr::getRandomFloat();

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
    for (int i = 0; i < kNumTests; i++) {
        int randomInt = ccr::getRandomInt(0, kUpperLimit);
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
    ccr::seedRng(123);
    printFloatTest();
    printIntTest();

    return EXIT_SUCCESS;
}
