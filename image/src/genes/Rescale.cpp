#include "../../include/genes/Rescale.hpp"

#include <opencv2/core/core.hpp>

#include <stdexcept>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::RescaleGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minValue_ = std::stoi(geneParameters.at("minScale"));
    maxValue_ = std::stoi(geneParameters.at("maxScale"));

    if (geneParameters.count("scale") > 0) {
        scale_ = std::stoi(geneParameters.at("scale"));
    } else {
        mutateParameters();
    }
}

void ci::RescaleGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    const ci::RescaleGene* gene = dynamic_cast<const ci::RescaleGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Attempting to initialize RescaleGene with a gene of type '"
            + other->getGeneName()
            + "'.");
    }

    minScale_ = gene->minScale_;
    maxScale_ = gene->maxScale_;
    scale_ = gene->scale_;
}

void ci::RescaleGene::mutateParameters() {
    scale_ = randomNumberGenerator_->getRandomInt(minScale_, maxScale_);
}

void ci::RescaleGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    width_ = buffers[0]->getWidth();
    height_ = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width_*height_*i);

        cv::Mat input(height_, width_, CV_8UC1, inputData + offset);
        cv::Mat output(height_, width_, CV_8UC1, outputData + offset);

        cv::Mat tmp;
        cv::resize(
            input,
            tmp,
            cv::Size((input.cols/scale_), (input.rows/scale_)),
            cv::INTER_AREA);

        cv::resize(
            tmp,
            output,
            cv::Size(input.cols, input.rows),
            cv::INTER_LANCZOS4);
    }
}

std::string ci::RescaleGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "  cv::Mat tmp;\n"
        << "  \n"
        << "  cv::resize(\n"
        << "    $INPUT,\n"
        << "    tmp,\n"
        << "    cv::Size($INPUT.cols/$SCALE, $INPUT.rows/$SCALE)\n"
        << "    cv::INTER_AREA);\n"
        << "\n"
        << "  cv::resize(\n"
        << "    tmp,\n"
        << "    $OUTPUT,\n"
        << "    cv::Size($INPUT.cols, $INPUT.rows)\n"
        << "    cv::INTER_LANCZOS4);\n"
        << "}\n";

    std::string output = replaceAllFxn(
        codeTemplate.str(),
        "$INPUT",
        context.inputVariableNames[0]);

    output = replaceAllFxn(
        output,
        "$OUTPUT",
        context.outputVariableName);

    output = replaceAllFxn(
        output,
        "$SCALE",
        std::to_string(scale_));

    return output;
}

std::unordered_map<std::string, std::string> ci::RescaleGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minScale"] = std::to_string(minScale_);
    tmp["maxScale"] = std::to_string(maxScale_);
    tmp["scale"] = std::to_string(scale_);

    return tmp;
}
