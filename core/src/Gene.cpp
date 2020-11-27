#include "../include/Gene.hpp"

#include <algorithm>
#include <stdlib.h>

namespace cc = cgpExperiments::core;

void initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    inputBufferIndices_.resize(getNumInputs(), 0);

    initializeParametersFromConfig(geneParameters);
}

void initializeFromTemplateGene(const cc::Gene* other) {
    inputBufferIndices_.resize(other->inputBufferIndices_.size());
    for (size_t i = 0; i < inputBufferIndices_.size(); i++) {
        inputBufferIndices_[i] = other->inputBufferIndices_[i];
    }

    outputBufferIndex_ = other->outputBufferIndex_;

    initializeParametersFromTemplateGene(other);
}

std::unordered_set<std::string, std::string> cc::Gene::serialize() {
    std::unordered_set<std::string, std::string> serializedGene = serializeInternal();
    serializedGene["geneName"] = getGeneName();

    std::string inputBufferIndicesStr = "";
    for (int i = 0; i < inputBufferIndices_.size(); i++) {
        inputBufferIndicesStr += std::to_string(inputBufferIndices_[i]);
        if (i < inputBufferIndices_.size() - 1) {
            inputBufferIndicesStr += ",";
        }
    }
    serializedGene["inputBufferIndexes"] = inputBufferIndicesStr;
    serializedGene["numInputs"] = inputBufferIndices_.size();

    return serializedGene;
}

void cc::Gene::connectInput(int inputNumber, int inputBufferIndex) {
    if (inputNumber >= inputBufferIndices_.size()) {
        throw std::runtime_error(
            "Error: tried to connect to input "
            + std::to_string(inputNumber)
            + ", but gene "
            + getGeneName()
            + " only has "
            + std::to_string(inputBufferIndices_.size())
            + " inputs.");
    }

    inputBufferIndices_[inputNumber] = inputBufferIndex;
}

void cc::Gene::setOutputIndex(int outputBufferIndex) {
    outputBufferIndex_ = outputBufferIndex;
}

std::string cc::Gene::generateUnusedVariableName(cc::CodeGenerationContext_t& context) {
    // Don't use the same RandomNumberGenerator as the experiment.
    // Serialization should not affect determinism.
    auto randchar = []() -> char
    {
        const char charset[] =
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };

    bool generatedUnique = false;
    constexpr int variableLength = 8;
    std::string variableName(length,0);

    while (!generatedUnique) {
        std::generate_n(variableName.begin(), variableLength, randchar);
        generatedUnique = (context.variableNamesInUse.count(variableName) == 0);
    }

    context.variableNamesInUse.emplace(generatedUnique);
    return generatedUnique;
}
