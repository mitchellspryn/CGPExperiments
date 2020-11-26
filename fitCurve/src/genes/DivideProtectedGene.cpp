#include "../../include/genes/DivideProtectedGene.hpp"

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

void cf::DivideProtectedGene::initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void cf::DivideProtectedGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void cf::DivideProtectedGene::mutateParameters() {
    // nothing to mutate!
}

std::string cf::DivideProtectedGene::getGeneName() {
    return "divideProtected";
}

void cf::DivideProtectedGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* secondBuf = buffers[inputBufferIndices_[1]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        if (std::abs(secondBuf[i]) < 0.000001) {
            outputBuf[i] = 1;
        } else {
            outputBuf[i] = firstBuf[i] / secondBuf[i];
        }
    }
}

std::string cf::DivideProtectedGene::generateCode(cc::CodeGenerationContext_t& context) {
    std::string addLine = 
        context.outputVariableName 
        + "[i] = " 
        + context.inputVariableNames[0]
        + "[i] + "
        + context.inputVariableNames[1]
        + "[i];";
    
    std::string codeTemplate = 
        "for (int i = 0; i < num; i++) {\n"
    +   "  if (std::abs(" + context.inputVariableNames[1] + "[i]) < 0.000001) {"
    +   "    " + context.outputVariableName + "[i] = 1;\n"
    +   "  } else {\n"
    +   "    " + context.outputVariableName + "[i] = " + context.inputVariableNames[0] + "[i] /" + context.inputVariableNames[1] + "[i];\n";
    +   "}\n";

    return codeTemplate;
}

std::unordered_map<std::string, std::string> cf::DivideProtectedGene::serializeInternal() {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
