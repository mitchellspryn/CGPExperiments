#pragma once

#include <string>
#include <unordered_map>

#include "Genotype.hpp"

namespace cgpExperiments {
namespace core {

typedef struct CheckpointLogInformation {
    int cumulativeNumberOfEpochs;
    double cumulativeElapsedTimeUs;
    float bestFitness;
} CheckpointLogInformation_t;

class CheckpointSaver {
    public:
        CheckpointSaver(
                const std::unordered_map<std::string, std::string>& checkpointSaverParameters);

        void saveCheckpoint(
                const CheckpointLogInformation_t& checkpointLogInformation,
                const Genotype& bestGenotype);
    private:
        std::string outputRootDirectory_;
        std::string logFileName_;
        bool generateCode_;
        bool generateImage_;

        void appendLogInformation(
                const CheckpointLogInformation_t& checkpointLogInformation);

        void saveGenotypeToJson(
                const std::string& outputDirectory,
                const Genotype& genotype);

        void saveGenotypeToCode(
                const std::string& outputDirectory,
                const Genotype& genotype);

        void saveGenotypeImage(
                const std::string& outputDirectory,
                const Genotype& genotype);

        void parseParameters(
                const std::unordered_map<std::string, std::string>& checkpointSaverParameters);
};

}
}
