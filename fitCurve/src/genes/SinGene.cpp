#include "../../include/genes/SinGene.hpp"

#include <cmath>

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

void cf::SinGene::initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void cf::SinGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void cf::SinGene::mutateParameters() {
    // nothing to mutate!
}

std::string cf::SinGene::getGeneName() {
    return "add";
}

void cf::SinGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = std::sin(firstBuf[i]);
    }
}

std::string cf::SinGene::generateCode(cc::CodeGenerationContext_t& context) {
    std::string line = 
        context.outputVariableName 
        + "[i] = std::sin(" 
        + context.inputVariableNames[0]
        + "[i]);";
    
    std::string codeTemplate = 
        "for (int i = 0; i < num; i++) {\n"
    +   "   " + line + "\n"
    +   "}\n";

    return codeTemplate;
}

std::unordered_map<std::string, std::string> cf::SinGene::serializeInternal() {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
