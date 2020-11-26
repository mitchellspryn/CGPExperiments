#include "../../include/genes/CosGene.hpp"

#include <cmath>

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

void cf::CosGene::initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // No parameters to initialize!
}

void cf::CosGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // No parameters to initialize!
}

void cf::CosGene::mutateParameters() {
    // Nothing to mutate!
}

std::string cf::CosGene::getGeneName() {
    return "cos";
}

void cf::CosGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = std::cos(firstBuf[i]);
    }
}

std::string cf::CosGene::generateCode(cc::CodeGenerationContext_t& context) {
    std::string line = 
        context.outputVariableName 
        + "[i] = std::cos(" 
        + context.inputVariableNames[0]
        + "[i]);";
    
    std::string codeTemplate = 
        "for (int i = 0; i < num; i++) {\n"
    +   "   " + line + "\n"
    +   "}\n";

    return codeTemplate;
}

std::unordered_map<std::string, std::string> cf::CosGene::serializeInternal() {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
