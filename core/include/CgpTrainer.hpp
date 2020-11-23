#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "CheckpointSaver.hpp"
#include "DataChunk.hpp"
#include "Gene.hpp"
#include "GeneFactory.hpp"
#include "Genotype.hpp"
#include "ProgressPrinter.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

class CgpTrainer {
    public:
        CgpTrainer(
            const std::unordered_map<std::string, std::string>& genotypeParameters,
            std::shared_ptr<GeneFactory> geneFactory,
            std::shared_ptr<FitnessFunction> fitnessFunction,
            std::shared_ptr<ProgressPrinter> progressPrinter,
            std::shared_ptr<CheckpointSaver> checkpointSaver,
            const std::unordered_map<std::string, std::string> trainerParameters);

        void run();
        const GenoType& getBestGenotype();

};

}
}
