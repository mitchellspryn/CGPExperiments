#include "../../include/genes/Unsharpen.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::UnsharpenGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    initializeKernel();
}

void ci::UnsharpenGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    initializeKernel();
}

void ci::UnsharpenGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::UnsharpenGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::filter2D(
            input, 
            output,
            -1,
            kernel_);
    }
}

std::string ci::UnsharpenGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "  double kernelData[9] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};\n"
        << "  cv::Mat kernel = cv::Mat(3, 3, CV_64FC1, kernelData.data());\n"
        << "\n"
        << "  cv::filter2D($INPUT, $OUTPUT, -1, kernel);\n"
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

std::unordered_map<std::string, std::string> ci::UnsharpenGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}

void ci::UnsharpenGene::initializeKernel() {
    kernel_ = cv::Mat::ones(3, 3, CV_64FC1) * -1;
    kernel_.at<double>(1, 1) = 9;
}
