#include "../../include/genes/MultiplyC.hpp"

#include <opencv2/core/core.hpp>

#include <stdexcept>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::MultiplyCGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minValue_ = std::stoi(geneParameters.at("minValue"));
    maxValue_ = std::stoi(geneParameters.at("maxValue"));

    if (geneParameters.count("value") > 0) {
        value_ = std::stoi(geneParameters.at("value"));
    } else {
        mutateParameters();
    }
}

void ci::MultiplyCGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    const ci::MultiplyCGene* gene = dynamic_cast<const ci::MultiplyCGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Attempting to initialize MultiplyCGene with a gene of type '"
            + other->getGeneName()
            + "'.");
    }

    minValue_ = gene->minValue_;
    maxValue_ = gene->maxValue_;
    value_ = gene->value_;
}

void ci::MultiplyCGene::mutateParameters() {
    value_ = randomNumberGenerator_->getRandomInt(minValue_, maxValue_);
}

void ci::MultiplyCGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        output = input * value_;
    }
}

std::string ci::MultiplyCGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << context.outputVariableName
        << " = "
        << context.inputVariableNames[0]
        << " * "
        << std::to_string(value_)
        << ";\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::MultiplyCGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minValue"] = std::to_string(minValue_);
    tmp["maxValue"] = std::to_string(maxValue_);
    tmp["value"] = std::to_string(value_);

    return tmp;
}
