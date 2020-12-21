#include "../../include/genes/Laplace.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::LaplaceGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::LaplaceGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::LaplaceGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::LaplaceGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        // TODO: should we allow modification of the kernel size?
        // Other candidate is hard-coded 3?
        cv::Laplacian(input, output, -1, 3);
    }
}

std::string ci::LaplaceGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::Laplacian("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", -1, 3"
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::LaplaceGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
