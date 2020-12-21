#include "../../include/genes/Normalize.hpp"

#include <opencv2/core/core.hpp>

#include <sstream>

namespace cc = cgpNormalizeeriments::core;
namespace ci = cgpNormalizeeriments::image;

void ci::NormalizeGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::NormalizeGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::NormalizeGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::NormalizeGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::normalize(input, output,  0, 255, cv::NORM_MINMAX, -1);
    }
}

std::string ci::NormalizeGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "cv::normalize(
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", 0, 255, cv::NORM_MINMAX, -1);\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::NormalizeGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
