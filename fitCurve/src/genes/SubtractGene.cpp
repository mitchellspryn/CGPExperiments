#include "../../include/genes/SubtractGene.hpp"

#include <sstream>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

void fc::SubtractGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void fc::SubtractGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void fc::SubtractGene::mutateParameters() {
    // nothing to mutate!
}

std::string fc::SubtractGene::getGeneName() const {
    return "subtract";
}

void fc::SubtractGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* secondBuf = buffers[inputBufferIndices_[1]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = firstBuf[i] - secondBuf[i];
    }
}

std::string fc::SubtractGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::string line = 
        context.outputVariableName 
        + "[i] = " 
        + context.inputVariableNames[0]
        + "[i] - "
        + context.inputVariableNames[1]
        + "[i];";
    
    std::stringstream codeTemplate;
    codeTemplate <<
        "for (int i = 0; i < num; i++) {\n"
    <<   "   " << line << "\n"
    <<   "}\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> fc::SubtractGene::serializeInternal() const {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
