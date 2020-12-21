#include "../../include/genes/LocalMax.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include <limits>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::LocalMaxGene::initializeParametersFromConfig(
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

void ci::LocalMaxGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::LocalMaxGene* gene = dynamic_cast<const ci::LocalMaxGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing LocalMaxGene with gene of improper type '"
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
}

void ci::LocalMaxGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();
    if (f < 0.5) { 
        mutateNeighborhoodWidth();
    } else {
        mutateNeighborhoodHeight();
    }
}

void ci::LocalMaxGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        // TODO: is there a slick way to do this with opencv api?
        for (int cy = 0; cy < input.rows; cy++) {
            for (int cx = 0; cx < input.cols; cx++) {
                int minX = std::max(cx - (neighborhoodWidth_ / 2), 0);
                int maxX = std::min(cx + (neighborhoodWidth_ / 2), input.cols - 1);
                int minY = std::max(cy - (neighborhoodHeight_ / 2), 0);
                int maxY = std::min(cy + (neighborhoodHeight_ / 2), input.rows - 1);

                int maxVal = std::numeric_limits<int>::min();
                for (int y = minY; y <= maxY; y++) {
                    for (int x = minX; x <= maxX; x++) {
                        maxVal = std::max(maxVal, input.at<unsigned char>(y, x));
                    }
                }

                output.at<unsigned char>(cy, cx) = maxVal;
            }
        }
    }
}

std::string ci::LocalMaxGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "for (int cy = 0; cy < $INPUT.rows; cy++) {\n"
        << "  for (int cx = 0; cx < $INPUT.cols; cx++) {\n"
        << "    int minX = std::max(cx - ($NW / 2), 0);\n"
        << "    int maxX = std::max(cx - ($NW / 2), $INPUT.cols - 1);\n"
        << "    int minY = std::max(cy - ($NH / 2), 0);\n"
        << "    int maxY = std::max(cy - ($NH / 2), $INPUT.rows - 1);\n"
        << "\n"
        << "    int maxVal = std::numeric_limits<int>::min();\n"
        << "    for (int y = minY; y <= maxY; y++) {\n"
        << "      for (int x = minX; x <= maxX; x++) {\n"
        << "        maxVal = std::max(maxVal, $INPUT.at<unsigned char>(y, x));\n"
        << "      }\n"
        << "    }\n"
        << "\n"
        << "    $OUTPUT.at(cy, cx) = maxVal;\n"
        << "  }\n"
        << "}\n";

    std::string templateCode = codeTemplate.str();
    templateCode = replaceAllFxn(templateCode, "$OUTPUT", context.outputVariableName);
    templateCode = replaceAllFxn(templateCode, "$INPUT", context.inputVariableNames[0]);
    templateCode = replaceAllFxn(templateCode, "$NW", std::to_string(neighborhoodWidth_));
    templateCode = replaceAllFxn(templateCode, "$NH", std::to_string(neighborhoodHeight_));

    return templateCode;
}

std::unordered_map<std::string, std::string> ci::LocalMaxGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minNeighborhoodWidth"] = std::to_string(minNeighborhoodWidth_);
    tmp["minNeighborhoodHeight"] = std::to_string(minNeighborhoodHeight_);
    tmp["maxNeighborhoodWidth"] = std::to_string(maxNeighborhoodWidth_);
    tmp["maxNeighborhoodHeight"] = std::to_string(maxNeighborhoodHeight_);
    tmp["neighborhoodWidth"] = std::to_string(neighborhoodWidth_);
    tmp["neighborhoodHeight"] = std::to_string(neighborhoodHeight_);

    return tmp;
}

void ci::LocalMaxGene::mutateNeighborhoodWidth() {
    neighborhoodWidth_ = randomNumberGenerator_->getRandomInt(
        minNeighborhoodWidth_,
        maxNeighborhoodWidth_);
}

void ci::LocalMaxGene::mutateNeighborhoodHeight() {
    neighborhoodHeight_ = randomNumberGenerator_->getRandomInt(
        minNeighborhoodHeight_,
        maxNeighborhoodHeight_);
}

