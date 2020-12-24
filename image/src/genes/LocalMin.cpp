#include "../../include/genes/LocalMin.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include <limits>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::LocalMinGene::initializeParametersFromConfig(
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
    
    refreshKernel();
}

void ci::LocalMinGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::LocalMinGene* gene = dynamic_cast<const ci::LocalMinGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing LocalMinGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minNeighborhoodWidth_ = gene->minNeighborhoodWidth_;
    maxNeighborhoodWidth_ = gene->maxNeighborhoodWidth_;
    neighborhoodWidth_ = gene->neighborhoodWidth_;

    minNeighborhoodHeight_ = gene->minNeighborhoodHeight_;
    maxNeighborhoodHeight_ = gene->maxNeighborhoodHeight_;
    neighborhoodHeight_ = gene->neighborhoodHeight_;

    refreshKernel();
}

void ci::LocalMinGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();
    if (f < 0.5) { 
        mutateNeighborhoodWidth();
    } else {
        mutateNeighborhoodHeight();
    }

    refreshKernel();
}

void ci::LocalMinGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::erode(input, output, kernel_); 
    }
}

std::string ci::LocalMinGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    auto replaceAllFxn = [](
            std::string str, 
            const std::string& from, 
            const std::string& to) -> std::string {
        size_t startPos = 0;
        while((startPos = str.find(from, startPos)) != std::string::npos) {
            str.replace(startPos, from.length(), to);
            startPos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }; 

    codeTemplate 
        << "{\n"
        << "  cv::Mat kernel = cv::Mat::ones($NH, $NW, CV_8UC1);\n"
        << "  cv::dilate($INPUT, $OUTPUT, kernel);\n"
        << "}\n";

    std::string templateCode = codeTemplate.str();
    templateCode = replaceAllFxn(templateCode, "$OUTPUT", context.outputVariableName);
    templateCode = replaceAllFxn(templateCode, "$INPUT", context.inputVariableNames[0]);
    templateCode = replaceAllFxn(templateCode, "$NW", std::to_string(neighborhoodWidth_));
    templateCode = replaceAllFxn(templateCode, "$NH", std::to_string(neighborhoodHeight_));

    return templateCode;
}

std::unordered_map<std::string, std::string> ci::LocalMinGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minNeighborhoodWidth"] = std::to_string(minNeighborhoodWidth_);
    tmp["minNeighborhoodHeight"] = std::to_string(minNeighborhoodHeight_);
    tmp["maxNeighborhoodWidth"] = std::to_string(maxNeighborhoodWidth_);
    tmp["maxNeighborhoodHeight"] = std::to_string(maxNeighborhoodHeight_);
    tmp["neighborhoodWidth"] = std::to_string(neighborhoodWidth_);
    tmp["neighborhoodHeight"] = std::to_string(neighborhoodHeight_);

    return tmp;
}

void ci::LocalMinGene::mutateNeighborhoodWidth() {
    neighborhoodWidth_ = randomNumberGenerator_->getRandomInt(
        minNeighborhoodWidth_,
        maxNeighborhoodWidth_);
}

void ci::LocalMinGene::mutateNeighborhoodHeight() {
    neighborhoodHeight_ = randomNumberGenerator_->getRandomInt(
        minNeighborhoodHeight_,
        maxNeighborhoodHeight_);
}

void ci::LocalMinGene::refreshKernel() {
    kernel_ = cv::Mat::ones(neighborhoodHeight_, neighborhoodWidth_, CV_8UC1);
}

