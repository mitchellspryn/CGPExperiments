#include "../../include/genes/SobelY.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::SobelYGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::SobelYGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::SobelYGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::SobelYGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::Sobel(
            input, 
            output,
            -1,
            0,
            1);
    }
}

std::string ci::SobelYGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::Sobel("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", -1, 0, 1);\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::SobelYGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
