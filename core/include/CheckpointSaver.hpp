#pragma once

#include <string>
#include <unordered_map>

#include "DataChunk.hpp"
#include "Genotype.hpp"

namespace cgpExperiments {
namespace core {

typedef struct CheckpointLogInformation {
    int cumulativeNumberOfEpochs;
    double cumulativeElapsedTimeUs;
    float bestFitness;
    bool isFinal;
} CheckpointLogInformation_t;

class CheckpointSaver {
    public:
        CheckpointSaver(
                const std::unordered_map<std::string, std::string>& checkpointSaverParameters);

        void saveCheckpoint(
                const CheckpointLogInformation_t& checkpointLogInformation,
                const Genotype& bestGenotype,
                const DataChunk& predictions);
    private:
        std::string outputRootDirectory_;
        std::string logFileName_;
        bool generateCode_;
        bool generateImage_;

        // TODO: this is a lazy hack for now.
        // At some point, I'll need to write the code to do a prediction from disk.
        bool generatePredictions_;

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

        void savePredictions(
                const std::string& outputDirectory,
                const DataChunk& predictions);

        void parseParameters(
                const std::unordered_map<std::string, std::string>& checkpointSaverParameters);
};

}
}
