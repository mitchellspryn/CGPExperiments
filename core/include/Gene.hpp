#pragma once

#include <string>
#include <type_traits>
#include <unordered_map>

#include "DataChunk.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

template <class NumInputs>
class Gene {
    public:
        Gene(const std::unordered_map<std::string, std::string>& geneParameters) {
            static_assert(std::is_integral<NumInputs>::value, "Integral NumInputs required.");

            inputGeneIndexes_.resize(NumInputs, 0);
        }

        virtual ~Gene() {};
        virtual void mutateParameters(RandomNumberGenerator& rng) = 0;
        virtual std::string getGeneName() const = 0;
        virtual std::unique_ptr<Gene> createCopy() = 0;

        virtual void evaluate(std::vector<DataChunk>& buffers) = 0;
        std::string generateCode(CodeGenerationContext_t& context) const = 0;

        int getNumInputs() { return NumInputs; }
        std::unordered_set<std::string, std::string> serialize() const;
        void connectInput(int inputNumber, int geneIndex);

    protected:
        std::vector<int> inputGeneIndexes_;

        virtual std::unordered_set<std::string, std::string> serializeInternal() const = 0;

        std::string generateUnusedVariableName(CodeGenerationContext_t& context) const;
};

}
}
