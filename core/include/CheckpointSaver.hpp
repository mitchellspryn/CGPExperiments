#pragma once

#include "Genotype.hpp"

namespace cgpExperiments {
namespace core {

typedef struct CheckpointLogInformation {
    int numberOfIterations;
    std::chrono::high_resolution_clock::time_point experimentStartTime;
    float bestFitness;
} CheckpointLogInformation_t;

class CheckpointSaver {
    public:
        CheckpointSaver(const std::string& checkpointDirectory);
        void saveCheckpoint(
                const CheckpointLogInformation_t& checkpointLogInformation,
                const Genotype& bestGenotype);

        
};

}
}
