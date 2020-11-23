#pragma once

namespace cgpExperiments {
namespace core {

enum class GenerationLanguage {
    CPP = 0
};

typedef struct CodeGenerationContext {
    GenerationLanguage generationLanguage;
    std::vector<std::string> inputVariableNames;
    std::string outputVariableName;
    std::unordered_set<std::string> variableNamesInUse;
} CodeGenerationContext_t;

}
}
