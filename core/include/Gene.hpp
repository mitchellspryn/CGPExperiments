#pragma once

#include <string>
#include <unordered_map>

#include "DataChunk.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

class Gene {
    public:
        Gene(const std::unordered_map<std::string, std::string>& geneParameters); 
        virtual ~Gene() {};
        virtual void mutateParameters(RandomNumberGenerator& rng) = 0;
        virtual int getNumInputs() = 0;
        virtual std::string getGeneName() const = 0;
        virtual std::unique_ptr<Gene> createCopy() = 0;

        virtual void evaluate(std::vector<DataChunk>& buffers) = 0;

        std::unordered_set<std::string, std::string> serialize() const;
        std::string generateCode(CodeGenerationContext_t& context) const;

    protected:
        int geneIndex_;
        std::vector<int> inputGeneIndexes_;

        std::string generateUnusedVariableName(CodeGenerationContext_t& context) const;

        virtual std::unordered_set<std::string, std::string> serializeInternal() const = 0;
};

}
}
