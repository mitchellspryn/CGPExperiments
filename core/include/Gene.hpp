#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "DataChunk.hpp"
#include "RandomNumberGenerator.hpp"
#include "Types.hpp"

namespace cgpExperiments {
namespace core {

// A gene that is part of a genetic program.
// This is the primary base class that should be extended for custom genesets.
class Gene {
    public:
        virtual ~Gene() {};

        // Describes how to mutate the internal parameters of the gene.
        // If there are no parameters, then the function should do nothing.
        virtual void mutateParameters() = 0;

        // Gets the name of the gene. This should be unique across the geneset.
        virtual std::string getGeneName() const = 0;

        // Gets the type id of the gene. This should be unique across the geneset.
        virtual int getTypeId() const = 0;

        // Given a buffer of datachunks, evaluate the gene.
        // The gene should use buffers[inputBufferIndices_] as the inputs, and write to buffers[outputBufferIndex_]
        virtual void evaluate(std::vector<std::shared_ptr<DataChunk>>& buffers) = 0;

        // Generate c++ code that implements this gene's functionality.
        virtual std::string generateCode(CodeGenerationContext_t& context) const = 0;

        // Returns true if there are no internal parameters, false otherwise.
        virtual bool isParameterFree() const = 0;

        // Gets the number of inputs to the gene.
        virtual int getNumInputs() const = 0;

        // Gets the compute cost of running the gene. 
        // This could be useful for multi-objective loss. 
        virtual float getComputeCost() const = 0;

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

        // Serialize the gene to a flat map. 
        // Enough parameters should be saved such that calling initializeParametersFromConfig() with the returned map
        //   should fully initialize the gene.
        virtual std::unordered_map<std::string, std::string> serializeInternal() const = 0;

        // Initialize the gene with the provided parameters.
        virtual void initializeParametersFromConfig(
            const std::unordered_map<std::string, std::string>& geneParameters) = 0;

        // Given a gene of identical type, create a copy of the gene. 
        // This function should copy over all internal parameters from the other gene.
        virtual void initializeParametersFromTemplateGene(const Gene* other) = 0;

        std::string generateUnusedVariableName(CodeGenerationContext_t& context) const;
};

}
}
