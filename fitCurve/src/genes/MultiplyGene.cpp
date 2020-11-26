#include "../../include/genes/MultiplyGene.hpp"

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

void cf::MultiplyGene::initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void cf::MultiplyGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void cf::MultiplyGene::mutateParameters() {
    // nothing to mutate!
}

std::string cf::MultiplyGene::getGeneName() {
    return "multiply";
}

void cf::MultiplyGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* secondBuf = buffers[inputBufferIndices_[1]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = firstBuf[i] * secondBuf[i];
    }
}

std::string cf::MultiplyGene::generateCode(cc::CodeGenerationContext_t& context) {
    std::string line = 
        context.outputVariableName 
        + "[i] = " 
        + context.inputVariableNames[0]
        + "[i] + "
        + context.inputVariableNames[1]
        + "[i];";
    
    std::string codeTemplate = 
        "for (int i = 0; i < num; i++) {\n"
    +   "   " + line + "\n"
    +   "}\n";

    return codeTemplate;
}

std::unordered_map<std::string, std::string> cf::MultiplyGene::serializeInternal() {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
