#include "../../include/genes/ConstantOutputGene.hpp"

#include <stdexcept>

#include "../../../core/include/RandomNumberGenerator.hpp"

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

void cf::ConstantOutputGene::initializeFromParameters(
        const std::unordered_map<std::string, std::string>& geneParameters) {

    minValue_ = std::stof(geneParameters["minValue"]);
    maxValue_ = std::stof(geneParameters["maxValue"]);

    if (geneParameters.count("value") > 0) {
        value_ = std::stof(geneParameters["value"]);
    } else {
        mutateParameters();
    }
}

void cf::ConstantOutputGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    cf::ConstantOutputGene* gene = dynamic_cast<cf::ConstantOutputGene*> other;
    if (gene == nullptr) {
        throw std::runtime_error(
            "Attempting to initialize ConstantOutputGene with something that isn't of the correct type.");
    }

    minValue_ = gene_->minValue_;
    maxValue_ = gene_->maxValue_;
    value_ = gene_->value;
}

void cf::ConstantOutputGene::mutateParameters() {
    value_ = minValue_ + ((maxValue_-minValue_) * cc::randomNumberGenerator::getRandomFloat());
}

std::string cf::ConstantOutputGene::getGeneName() {
    return "constantOutput";
}

void cf::ConstantOutputGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    float* outputBuf = buffers[outputBufferIndex_]->getDataPtr();

    int numExamples = buffers[outputBufferIndex_]->getNum();
    for (int i = 0; i < numExamples; i++) {
        outputBuf[i] = value_;
    }
}

std::string cf::ConstantOutputGene::generateCode(cc::CodeGenerationContext_t& context) {
    std::string line = 
        context.outputVariableName 
        + "[i] = " 
        + std::to_string(value)
        + ";";
    
    std::string codeTemplate = 
        "for (int i = 0; i < num; i++) {\n"
    +   "   " + line + "\n"
    +   "}\n";

    return codeTemplate;
}

std::unordered_map<std::string, std::string> cf::ConstantOutputGene::serializeInternal() {
    std::unordered_map<std::string, std::string> tmp;

    tmp["maxValue"] = std::to_string(maxValue_);
    tmp["minValue"] = std::to_string(minValue_);
    tmp["value"] = std::to_string(value_);

    return tmp;
}
