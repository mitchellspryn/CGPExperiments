#include "../../include/genes/AbsDifference.hpp"

#include <opencv2/core/core.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::AbsDifferenceGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::AbsDifferenceGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::AbsDifferenceGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::AbsDifferenceGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* aData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* bData = buffers[inputBufferIndices_[1]]->getCharDataPtr();
    unsigned char* cData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat a(height, width, CV_8UC1, aData + offset);
        cv::Mat b(height, width, CV_8UC1, bData + offset);
        cv::Mat c(height, width, CV_8UC1, cData + offset);

        cv::absdiff(a, b, c);
    }
}

std::string ci::AbsDifferenceGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::absdiff("
        << context.inputVariableNames[0]
        << ", "
        << context.inputVariableNames[1]
        << ", "
        << context.outputVariableName
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::AbsDifferenceGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
