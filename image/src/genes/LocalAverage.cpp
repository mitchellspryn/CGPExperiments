#include "../../include/genes/LocalAverage.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::LocalAverageGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minNeighborhoodWidth_ = std::stoi(geneParameters.at("minNeighborhoodWidth"));
    minNeighborhoodHeight_ = std::stoi(geneParameters.at("minNeighborhoodHeight"));
    maxNeighborhoodWidth_ = std::stoi(geneParameters.at("maxNeighborhoodWidth"));
    maxNeighborhoodHeight_ = std::stoi(geneParameters.at("maxNeighborhoodHeight"));

    if (geneParameters.count("neighborhoodWidth") > 0) {
        neighborhoodWidth_ = std::stoi(geneParameters.at("neighborhoodWidth"));
    } else {
        mutateNeighborhoodWidth();
    }

    if (geneParameters.count("neighborhoodHeight") > 0) {
        neighborhoodHeight_ = std::stoi(geneParameters.at("neighborhoodHeight"));
    } else {
        mutateNeighborhoodHeight();
    }

}

void ci::LocalAverageGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::LocalAverageGene* gene = dynamic_cast<const ci::LocalAverageGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing LocalAverageGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minNeighborhoodWidth_ = gene->minNeighborhoodWidth_;
    maxNeighborhoodWidth_ = gene->maxNeighborhoodWidth_;
    neighborhoodWidth_ = gene->neighborhoodWidth_;

    minNeighborhoodHeight_ = gene->minNeighborhoodHeight_;
    maxNeighborhoodHeight_ = gene->maxNeighborhoodHeight_;
    neighborhoodHeight_ = gene->neighborhoodHeight_;

    initializeStructuringElement();

void ci::LocalAverageGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();
    if (f < 0.5) { 
        mutateNeighborhoodWidth();
    } else {
        mutateNeighborhoodHeight();
    }
}

void ci::LocalAverageGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::boxFilter(
            input, 
            output, 
            -1,
            cv::Size(neighborhoodWidth_, neighborhoodHeight_), 
            cv::Point(-1, -1),
            true);
    }
}

std::string ci::LocalAverageGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::boxFilter("
        << context.inputVariableNames[0]
        << ","
        << context.outputVariableName
        << ", -1, cv::Size("
        << std::to_string(neighborhoodWidth_)
        << ", "
        << std::to_string(neighborhoodHeight_)
        << "), cv::Point(-1, -1), true"
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::LocalAverageGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minNeighborhoodWidth"] = std::to_string(minNeighborhoodWidth_);
    tmp["minNeighborhoodHeight"] = std::to_string(minNeighborhoodHeight_);
    tmp["maxNeighborhoodWidth"] = std::to_string(maxNeighborhoodWidth_);
    tmp["maxNeighborhoodHeight"] = std::to_string(maxNeighborhoodHeight_);
    tmp["neighborhoodWidth"] = std::to_string(neighborhoodWidth_);
    tmp["neighborhoodHeight"] = std::to_string(neighborhoodHeight_);

    return tmp;
}

void ci::LocalAverageGene::mutateNeighborhoodWidth() {
    neighborhoodWidth_ = randomNumberGenerator_->getRandomInt(
        minNeighborhoodWidth_,
        maxNeighborhoodWidth_);
}

void ci::LocalAverageGene::mutateNeighborhoodHeight() {
    neighborhoodHeight_ = randomNumberGenerator_->getRandomInt(
        minNeighborhoodHeight_,
        maxNeighborhoodHeight_);
}

