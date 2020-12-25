#include "../../include/genes/SmoothMedian.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::SmoothMedianGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minKernelSize_ = std::stoi(geneParameters.at("minKernelSize"));
    maxKernelSize_ = std::stoi(geneParameters.at("maxKernelSize"));

    if (geneParameters.count("kernelSize") > 0) {
        kernelSize_ = std::stoi(geneParameters.at("kernelSize"));
    } else {
        mutateParameters();
    }
}

void ci::SmoothMedianGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::SmoothMedianGene* gene = dynamic_cast<const ci::SmoothMedianGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing SmoothMedianGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minKernelSize_ = gene->minKernelSize_;
    maxKernelSize_ = gene->maxKernelSize_;
    kernelSize_ = gene->kernelSize_;
}


void ci::SmoothMedianGene::mutateParameters() {
    kernelSize_ = randomNumberGenerator_->getRandomInt(minKernelSize_, maxKernelSize_);
    if ((kernelSize_ % 2) == 0) {
        float f = randomNumberGenerator_->getRandomFloat();
        if (f > 0.5) {
            kernelSize_ -= 1;
        } else {
            kernelSize_ += 1;
        }
    }
}

void ci::SmoothMedianGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::medianBlur(
            input, 
            output, 
            kernelSize_);
    }
}

std::string ci::SmoothMedianGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::medianBlur("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", "
        << std::to_string(kernelSize_)
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::SmoothMedianGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minKernelSize"] = std::to_string(minKernelSize_);
    tmp["maxKernelSize"] = std::to_string(maxKernelSize_);
    tmp["kernelSize"] = std::to_string(kernelSize_);

    return tmp;
}

