#include "../../include/genes/Threshold.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::ThresholdGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minThresh_ = std::stoi(geneParameters.at("minThresh"));
    maxThresh_ = std::stoi(geneParameters.at("maxThresh"));

    if (geneParameters.count("thresh") > 0) {
        thresh_ = std::stoi(geneParameters.at("thresh"))
    } else {
        mutateParameters();
    }
}

void ci::ThresholdGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::ThresholdGene* gene = dynamic_cast<const ci::ThresholdGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing ThresholdGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minThresh_ = gene->minThresh_;
    maxThresh_ = gene->maxThresh_;
    thresh_ = gene->thresh_;
}

void ci::ThresholdGene::mutateParameters() {
    thresh_ = randomNumberGenerator_->getRandomInt(minThresh_, maxThresh_);
}

void ci::ThresholdGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::threshold(input, output, thresh_, 255, cv::THRESH_BINARY);
    }
}

std::string ci::ThresholdGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::thershold("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", "
        << std::to_string(thresh_)
        << ", 255, cv::THRESH_BINARY"
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::ThresholdGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minThresh"] = std::to_string(minThresh_);
    tmp["maxThresh"] = std::to_string(maxThresh_);
    tmp["thresh"] = std::to_string(thresh_);

    return tmp;
}
