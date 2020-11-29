#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "DataChunk.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

class Gene {
    public:
        virtual ~Gene() {};
        virtual void mutateParameters() = 0;
        virtual std::string getGeneName() const = 0;

        virtual int getTypeId() const = 0;
        virtual void evaluate(std::vector<std::shared_ptr<DataChunk>>& buffers) = 0;
        virtual std::string generateCode(CodeGenerationContext_t& context) const = 0;
        virtual bool isParameterFree() const = 0;
        virtual int getNumInputs() const = 0;

        const std::vector<int>& getInputBufferIndices() { return inputBufferIndices_; }

        void initializeFromParameters(const std::unordered_map<std::string, std::string>& geneParameters);
        void initializeFromTemplateGene(const Gene* other);
        std::unordered_map<std::string, std::string> serialize() const;
        void connectInput(int inputNumber, int inputBufferIndex);
        void setOutputIndex(int outputBufferIndex);

        // Do not use shared_ptr, as this will be called in the hot loop
        // The RNG will be owned by the genotype, as the gene cannot exist outside of it.
        void setRandomNumberGenerator(RandomNumberGenerator* rng);

    protected:
        int outputBufferIndex_;
        std::vector<int> inputBufferIndices_;
        RandomNumberGenerator* randomNumberGenerator_ = nullptr;

        virtual std::unordered_map<std::string, std::string> serializeInternal() const = 0;
        virtual void initializeParametersFromConfig(
            const std::unordered_map<std::string, std::string>& geneParameters) = 0;
        virtual void initializeParametersFromTemplateGene(const Gene* other) = 0;

        std::string generateUnusedVariableName(CodeGenerationContext_t& context) const;
};

}
}
