#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "CheckpointSaver.hpp"
#include "DataChunk.hpp"
#include "Gene.hpp"
#include "GeneFactory.hpp"
#include "GenePool.hpp"
#include "Genotype.hpp"
#include "Island.hpp"
#include "ProgressPrinter.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

class CgpTrainer {
    public:
        CgpTrainer(
            std::shared_ptr<FitnessFunctionFactory> fitnessFunctionFactory,
            std::shared_ptr<GeneFactory> geneFactory,
            std::shared_ptr<ExperimentConfiguration> experimentConfiguration);

        void run();
        const Genotype& getBestGenotype();
        double getBestGenotypeFitness();

    private:
        int maxNumThreads_;
        int numIslands_;
        int rngSeed_;
        double terminationFitness_;
        double terminationNumIterations_;

        int bestIslandIndex_;
        double bestFitnessScore_;

        std::shared_ptr<FitnessFunctionFactory> fitnessFunctionFactory_;
        std::shared_ptr<GenePool> genePool_;
        std::shared_ptr<ExperimentConfiguration> experimentConfiguration_;
        std::vector<Island> islands_;

        std::vector<std::shared_ptr<DataChunkProvider>> inputDataChunkProviders_;
        std::shared_ptr<DataChunkProvider> labelDataChunkProvider_;

        void fillParametersFromMap(const std::unordered_map<std::string, std::string>& parameters);
};

}
}
