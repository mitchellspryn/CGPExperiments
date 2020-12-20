#include "../../include/genes/Canny.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::CannyGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minLowerThresh_ = std::stoi(geneParameters.at("minLowerThresh"));
    maxLowerThresh_ = std::stoi(geneParameters.at("maxLowerThresh"));
    minUpperThresh_ = std::stoi(geneParameters.at("minUpperThresh"));
    maxUpperThresh_ = std::stoi(geneParameters.at("maxUpperThresh"));

    // Some sanity checks
    if (maxLowerThresh_ > maxUpperThresh_) {
        throw std::runtime_error("MaxLowerThresh cannot be larger than MaxUpperThresh.");
    }

    if (minLowerThresh_ > minUpperThresh_) {
        throw std::runtime_error("MinLowerThresh cannot be larger than MinUpperThresh.");
    }   

    if (geneParameters.count("lowerThresh") > 0) {
        lowerThresh_ = std::stoi(geneParameters.at("lowerThresh"));
    } else {
        lowerThresh_ = randomNumberGenerator_->getRandomInt(minLowerThresh_, maxLowerThresh_);
    }

    if (geneParameters.count("upperThresh") > 0) {
        upperThresh_ = std::stoi(geneParameters.at("upperThresh"));
    } else {
        upperThresh_ = randomNumberGenerator_->getRandomInt(minUpperThresh_, maxUpperThresh_);
    }

    rectifyThresholds();
}

void ci::CannyGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::CannyGene* gene = dynamic_cast<const ci::CannyGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing CannyGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minLowerThresh_ = gene->minLowerThresh_;
    minUpperThresh_ = gene->minUpperThresh_;
    maxLowerThresh_ = gene->maxLowerThresh_;
    maxUpperThresh_ = gene->maxUpperThresh_;
    lowerThresh_ = gene->lowerThresh_;
    upperThresh_ = gene->upperThresh_;
}

void ci::CannyGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();

    // TODO: should I allow a single mutation to mutate both?
    if (f < 0.5) {
        lowerThresh_ = randomNumberGenerator_->getRandomInt(minLowerThresh_, maxLowerThresh_);
    } else {
        upperThresh_ = randomNumberGenerator_->getRandomInt(minUpperThresh_, maxUpperThresh_);
    }

    rectifyThresholds();
}

void ci::CannyGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::canny(input, output, lowerThresh_, upperThresh_);
    }
}

std::string ci::CannyGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::canny("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", "
        << std::to_string(lowerThresh_)
        << ", "
        << std::to_string(upperThresh_)
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::CannyGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minLowerThresh"] = std::to_string(minLowerThresh_);
    tmp["minUpperThresh"] = std::to_string(minUpperThresh_);
    tmp["maxLowerThresh"] = std::to_string(maxLowerThresh_);
    tmp["maxUpperThresh"] = std::to_string(maxUpperThresh_);
    tmp["lowerThresh"] = std::to_string(lowerThresh_);
    tmp["upperThresh"] = std::to_string(upperThresh_);

    return tmp;
}

void ci::CannyGene::rectifyThresholds() {
    if (lowerThresh_ > upperThresh_) {
        std::swap(lowerThresh_, upperThresh_);
    }
}
