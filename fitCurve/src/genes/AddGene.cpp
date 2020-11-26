#include "../../include/genes/AddGene.hpp"

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

void cf::AddGene::initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void cf::AddGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void cf::AddGene::mutateParameters() {
    // nothing to mutate!
}

std::string cf::AddGene::getGeneName() {
    return "add";
}

void cf::AddGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* secondBuf = buffers[inputBufferIndices_[1]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = firstBuf[i] + secondBuf[i];
    }
}

std::string cf::AddGene::generateCode(cc::CodeGenerationContext_t& context) {
    std::string addLine = 
        context.outputVariableName 
        + "[i] = " 
        + context.inputVariableNames[0]
        + "[i] + "
        + context.inputVariableNames[1]
        + "[i];";
    
    std::string codeTemplate = 
        "for (int i = 0; i < num; i++) {\n"
    +   "   " + addLine + "\n"
    +   "}\n";

    return codeTemplate;
}

std::unordered_map<std::string, std::string> cf::AddGene::serializeInternal() {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
