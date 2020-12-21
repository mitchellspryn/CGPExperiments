#pragma once

#include <string>
#include <unordered_set>
#include <vector>

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
    int inputWidth;
    int inputHeight;
    int inputN;
} CodeGenerationContext_t;

}
}
