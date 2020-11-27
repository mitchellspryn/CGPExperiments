#pragma once

#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ExperimentConfiguration.hpp"
#include "FitnessFunction.hpp"
#include "FitnessFunctionFactory.hpp"
#include "Genotype.hpp"
#include "GeneFactory.hpp"
#include "RandomNumberGenerator.hpp"

namespace cgpExperiments {
namespace core {

class Island {
    public:
        Island(
                std::shared_ptr<FitnessFunctionFactory> fitnessFunctionFactory,
                std::shared_ptr<GenePool> genePool,
                const std::vector<std::shared_ptr<DataChunkProvider>>& inputDataChunkProviders,
                const std::shared_ptr<DataChunkProvider>& labelDataChunkProvider,
                std::shared_ptr<ExperimentConfiguration> experimentConfiguration);

        int getNumIterationsPerEpoch();
        void runEpoch();
        double getBestFitness();
        const Genotype& getBestGenotype();
        void setResidentGenotypes(const Genotype& genotype, double fitness);

    private:
        int numGenotypes_;
        int numIterationsPerEpoch_;
        int numEvaluationSamples_;
        int evalStartIndex_;
        int checkpointFrequency_;
        double bestFitness_ = std::numeric_limits<double>::max();
        int bestFitnessIndex_ = -1;

        std::shared_ptr<ExperimentConfiguration> experimentConfiguration_;
        std::unique_ptr<FitnessFunction> fitnessFunction_;
        std::vector<Genotype> residents_;
        std::vector<std::shared_ptr<DataChunkProvider>> inputDataChunkProviders_;
        std::shared_ptr<DataChunkProvider> labelDataChunkProvider_;
        std::vector<std::unique_ptr<DataChunk>> inputDataChunkBuffers_;
        std::unique_ptr<DataChunk> labelDataChunkBuffer_;

        void fillParametersFromMap(
                const std::unordered_map<std::string, std::string>& params);
};

}
}


