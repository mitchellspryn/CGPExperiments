#pragma once

#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "DataChunk.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

template <typename NumInputs>
class Gene {
    public:
        Gene(const std::unordered_map<std::string, std::string>& geneParameters,
                std::shared_ptr<RandomNumberGenerator> rng) {
            static_assert(std::is_integral<NumInputs>::value, "Integral NumInputs required.");

            inputGeneIndexes_.resize(NumInputs, 0);
            rng_ = rng;
        }

        virtual ~Gene() {};
        virtual void mutateParameters() = 0;
        virtual std::string getGeneName() const = 0;
        virtual std::unique_ptr<Gene> createCopy() = 0;

        virtual void evaluate(std::vector<DataChunk>& buffers) = 0;
        std::string generateCode(CodeGenerationContext_t& context) const = 0;

        int getNumInputs() { return NumInputs; }
        const std::vector<int>& getInputBufferIndices() { return inputBufferIndices_; }
        std::unordered_set<std::string, std::string> serialize() const;
        void connectInput(int inputNumber, int inputBufferIndex);
        void setOutputIndex(int outputBufferIndex);

    protected:
        int outputBufferIndex_;
        std::vector<int> inputBufferIndices_;
        std::shared_ptr<RandomNumberGenerator> rng_;

        virtual std::unordered_set<std::string, std::string> serializeInternal() const = 0;

        std::string generateUnusedVariableName(CodeGenerationContext_t& context) const;
};

}
}
