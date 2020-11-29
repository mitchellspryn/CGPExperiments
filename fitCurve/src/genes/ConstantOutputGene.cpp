#include "../../include/genes/ConstantOutputGene.hpp"

#include <sstream>
#include <stdexcept>

#include "../../../core/include/RandomNumberGenerator.hpp"

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

void fc::ConstantOutputGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {

    minValue_ = std::stof(geneParameters.at("minValue"));
    maxValue_ = std::stof(geneParameters.at("maxValue"));

    if (geneParameters.count("value") > 0) {
        value_ = std::stof(geneParameters.at("value"));
    } else {
        mutateParameters();
    }
}

void fc::ConstantOutputGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    const fc::ConstantOutputGene* gene = dynamic_cast<const fc::ConstantOutputGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Attempting to initialize ConstantOutputGene with something that isn't of the correct type.");
    }

    minValue_ = gene->minValue_;
    maxValue_ = gene->maxValue_;
    value_ = gene->value_;
}

void fc::ConstantOutputGene::mutateParameters() {
    value_ = minValue_ + ((maxValue_-minValue_) * randomNumberGenerator_->getRandomFloat());
}

std::string fc::ConstantOutputGene::getGeneName() const {
    return "constantOutput";
}

void fc::ConstantOutputGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[outputBufferIndex_]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = value_;
    }
}

std::string fc::ConstantOutputGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::string line = 
        context.outputVariableName 
        + "[i] = " 
        + std::to_string(value_)
        + ";";
    
    std::stringstream codeTemplate;
    codeTemplate <<
        "for (int i = 0; i < num; i++) {\n"
    <<  "   " << line << "\n"
    <<  "}\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> fc::ConstantOutputGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["maxValue"] = std::to_string(maxValue_);
    tmp["minValue"] = std::to_string(minValue_);
    tmp["value"] = std::to_string(value_);

    return tmp;
}
