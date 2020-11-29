#pragma once

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "DataChunk.hpp"
#include "ExperimentConfiguration.hpp"
#include "Gene.hpp"
#include "GenePool.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

enum class MutationType {
    Percentage = 0,
    Probability,
    SingleActive
};

class Genotype {
    public:
        Genotype(
                std::shared_ptr<ExperimentConfiguration> experimentConfiguration,
                std::shared_ptr<GenePool> genePool,
                std::shared_ptr<RandomNumberGenerator> randomNumberGenerator);

        void mutate();
        void setGenes(
                const std::vector<std::unique_ptr<Gene>>& modelGenes, 
                const std::unordered_set<int>& activeGenes);
        const std::vector<std::unique_ptr<Gene>>& getGenes() const;
        const std::unordered_set<int> getActiveGenes() const;
        const DataChunk& evaluate(std::vector<std::shared_ptr<DataChunk>>& inputs);
        std::vector<std::unordered_map<std::string, std::string>> serialize() const;
        void deserialize(const std::vector<std::unordered_map<std::string, std::string>>& data);
        std::string generateCode(CodeGenerationContext_t& context) const;
        std::string generateDotFile(bool includeUnusedNodes) const;
        void initializeRandomly();
        void randomlyReconnectGeneInput(int inputNumber, int geneIndex);

    private:
        int geneGridWidth_;
        int geneGridHeight_;
        int maxLookback_;
        int numInputDatasets_;
        int inputDataWidth_;
        int inputDataHeight_;
        int inputDataNumSamples_;

        int outputBufferIndex_;

        MutationType mutationType_;
        double mutationPercentage_;
        int mutationPercentageNumGenes_;
        double mutationProbability_;

        std::vector<std::unique_ptr<Gene>> genes_;
        std::vector<std::shared_ptr<DataChunk>> buffers_;
        std::stack<int> indicesToEvaluate_;

        // TODO: should this be a bool vec?
        std::unordered_set<int> activeGenes_;

        std::shared_ptr<ExperimentConfiguration> experimentConfiguration_;
        std::shared_ptr<GenePool> genePool_;
        std::shared_ptr<RandomNumberGenerator> randomNumberGenerator_;

        void mutateUntilPercentage();
        void mutateByProbability();
        void mutateUntilSingleActive();
        void mutateSingleGene(int geneIndex);

        void fillParametersFromMap(
                const std::unordered_map<std::string, std::string>& params);
};

}
}
