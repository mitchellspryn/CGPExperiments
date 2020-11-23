#include "../include/Gene.hpp"

#include <algorithm>
#include <stdlib.h>

namespace cc = cgpExperiments::core;

std::unordered_set<std::string, std::string> cc::Gene::serialize() {
    std::unordered_set<std::string, std::string> serializedGene = serializeInternal();
    serializedGene["geneName"] = getGeneName();
    return serializedGene;
}

void cc::Gene::connectInput(int inputNumber, int geneIndex) {
    if (inputNumber >= inputGeneIndexes_.size()) {
        throw std::runtime_error(
            "Error: tried to connect to input "
            + std::to_string(inputNumber)
            + ", but gene "
            + getGeneName()
            + " only has "
            + std::to_string(inputGeneIndexes_.size())
            + " inputs.");
    }

    inputGeneIndexes_[inputNumber] = geneIndex;
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
