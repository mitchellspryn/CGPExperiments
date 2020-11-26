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
        Gene() {
            static_assert(std::is_integral<NumInputs>::value, "Integral NumInputs required.");

            inputBufferIndices_.resize(NumInputs, 0);
        }

        virtual ~Gene() {};
        virtual void initializeFromParameters(const std::unordered_map<std::string, std::string>& geneParameters) = 0;
        virtual void mutateParameters() = 0;
        virtual std::string getGeneName() const = 0;

        virtual void evaluate(std::vector<std::shared_ptr<DataChunk>>& buffers) = 0;
        virtual std::string generateCode(CodeGenerationContext_t& context) const = 0;

        int getNumInputs() { return NumInputs; }
        const std::vector<int>& getInputBufferIndices() { return inputBufferIndices_; }

        void initializeFromTemplateGene(const Gene* other) = 0;
        std::unordered_map<std::string, std::string> serialize() const;
        void connectInput(int inputNumber, int inputBufferIndex);
        void setOutputIndex(int outputBufferIndex);

    protected:
        int outputBufferIndex_;
        std::vector<int> inputBufferIndices_;

        virtual std::unordered_map<std::string, std::string> serializeInternal() const = 0;
        virtual void initializeParametersFromTemplateGene(const Gene* other) = 0;

        std::string generateUnusedVariableName(CodeGenerationContext_t& context) const;
};

}
}
