#include "../../include/genes/MinValue.hpp"

#include <opencv2/core/core.hpp>

#include <sstream>

namespace cc = cgpMinValueeriments::core;
namespace ci = cgpMinValueeriments::image;

void ci::MinValueGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::MinValueGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::MinValueGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::MinValueGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        double minVal;
        double maxVal;
        cv::minMaxLoc(input, &minVal, &maxVal);

        output.setTo(cv::Scalar(minVal));
    }
}

std::string ci::MinValueGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "  double minVal;\n"
        << "  double maxVal;\n"
        << "  cv::minMaxLoc($INPUT, &minVal, &maxVal);\n"
        << "  $OUTPUT.setTo(cv::Scalar(minVal));\n"
        << "}\n";

    std::string output = replaceAllFxn(
        codeTemplate.str(),
        "$INPUT",
        context.inputVariableNames[0]);

    output = replaceAllFxn(
        output,
        "$OUTPUT",
        context.outputVariableName);

    return output;
}

std::unordered_map<std::string, std::string> ci::MinValueGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
