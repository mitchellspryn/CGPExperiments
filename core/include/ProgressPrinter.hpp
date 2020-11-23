#pragma once

#include <chrono>

namespace cgpExperiments {
namespace core {

typedef struct ProgressUpdate {
    int numberOfIterations;
    std::chrono::high_resolution_clock::time_point experimentStartTime;
    float bestFitness;
} ProgressUpdate_t;

class ProgressPrinter {
    public:
        void PrintProgressToConsole(const ProgressUpdate_t& update) const;
};

}
}
