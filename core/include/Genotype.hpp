#pragma once

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "DataChunk.hpp"
#include "Gene.hpp"
#include "GeneFactory.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

class Genotype {
    public:
        Genotype(
                const std::unordered_map<std::string, std::string>& genotypeParameters,
                std::shared_ptr<GeneFactory> geneFactory,
                std::shared_ptr<FitnessFunction> fitnessFunction);

        void mutate(RandomNumberGenerator& rng);
        const DataChunk& evaluate(int sampleStartIndex=-1);
        std::vector<std::unordered_set<std::string, std::string>> serialize() const;
        std::string generateCode(CodeGenerationContext_t& context) const;

    private:
        std::vector<std::unique_ptr<Gene>> genes_;
        std::vector<std::unique_ptr<DataChunkProvider>> inputDataProviders_;
        std::vector<std::unique_ptr<DataChunk>> buffers_;
        std::stack<int> indicesToEvaluate_;
        std::unordered_set<int> activeGenomes_;

};

}
}
