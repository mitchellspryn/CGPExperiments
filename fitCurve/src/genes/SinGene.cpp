#include "../../include/genes/SinGene.hpp"

#include <cmath>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

void fc::SinGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void fc::SinGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void fc::SinGene::mutateParameters() {
    // nothing to mutate!
}

std::string fc::SinGene::getGeneName() const {
    return "add";
}

void fc::SinGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* firstBuf = buffers[inputBufferIndices_[0]]->getDataPtr();
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[inputBufferIndices_[0]]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = std::sin(firstBuf[i]);
    }
}

std::string fc::SinGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::string line = 
        context.outputVariableName 
        + "[i] = std::sin(" 
        + context.inputVariableNames[0]
        + "[i]);";
    
    std::stringstream codeTemplate;
    codeTemplate <<
        "for (int i = 0; i < num; i++) {\n"
    <<  "   " << line << "\n"
    <<  "}\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> fc::SinGene::serializeInternal() const {
    // No parameters to serialize here!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
