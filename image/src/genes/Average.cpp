#include "../../include/genes/Average.hpp"

#include <opencv2/core/core.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::AverageGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::AverageGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::AverageGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::AverageGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
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

        c = ((0.5 * a) + (0.5 * b));
    }
}

std::string ci::AverageGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << context.outputVariableName
        << " = ((0.5 * "
        << context.inputVariableNames[0]
        << ") + (0.5 * "
        << context.inputVariableNames[1]
        << "));\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::AverageGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
