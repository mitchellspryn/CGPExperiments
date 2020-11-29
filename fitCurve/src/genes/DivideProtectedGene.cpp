#include "../../include/genes/DivideProtectedGene.hpp"

#include <sstream>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

void fc::DivideProtectedGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void fc::DivideProtectedGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void fc::DivideProtectedGene::mutateParameters() {
    // nothing to mutate!
}

std::string fc::DivideProtectedGene::getGeneName() const {
    return "divideProtected";
}

void fc::DivideProtectedGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
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

std::string fc::DivideProtectedGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::string addLine = 
        context.outputVariableName 
        + "[i] = " 
        + context.inputVariableNames[0]
        + "[i] + "
        + context.inputVariableNames[1]
        + "[i];";
    
    std::stringstream codeTemplate;
    codeTemplate <<
        "for (int i = 0; i < num; i++) {\n"
    <<  "  if (std::abs(" << context.inputVariableNames[1] << "[i]) < 0.000001) {\n"
    <<  "    " << context.outputVariableName << "[i] = 1;\n"
    <<  "  } else {\n"
    <<  "    " << context.outputVariableName << "[i] = " << context.inputVariableNames[0] << "[i] / " << context.inputVariableNames[1] << "[i];\n"
    <<  "}\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> fc::DivideProtectedGene::serializeInternal() const {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
