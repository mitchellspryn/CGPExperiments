#include "../../include/genes/Erode.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::ErodeGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minNumIterations_ = std::stoi(geneParameters.at("minNumIterations"));
    maxNumIterations_ = std::stoi(geneParameters.at("maxNumIterations"));

    if (geneParameters.count("numIterations") > 0) {
        numIterations_ = std::stoi(geneParameters.at("numIterations"));
    } else {
        mutateParameters();
    }

    initializeStructuringElement();
}

void ci::ErodeGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::ErodeGene* gene = dynamic_cast<const ci::ErodeGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing ErodeGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minNumIterations_ = gene->minNumIterations_;
    maxNumIterations_ = gene->maxNumIterations_;
    numIterations_ = gene->numIterations_;

    initializeStructuringElement();
}

void ci::ErodeGene::mutateParameters() {
    numIterations_ = randomNumberGenerator_->getRandomInt(minNumIterations_, maxNumIterations_);
}

void ci::ErodeGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::erode(input, output, structuringElement_, cv::Point(-1, -1), numIterations_);
    }
}

std::string ci::ErodeGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::erode("
        << context.inputVariableNames[0]
        << ", cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)),"
        << context.outputVariableName
        << ", cv::Point(-1, -1), "
        << std::to_string(numIterations_)
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::ErodeGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minNumIterations"] = std::to_string(minNumIterations_);
    tmp["maxNumIterations"] = std::to_string(maxNumIterations_);
    tmp["numIterations"] = std::to_string(numIterations_);

    return tmp;
}

void ci::ErodeGene::initializeStructuringElement() {
    // TODO: should we allow for changing the structuring element of dilation?
    structuringElement_ = cv::getStructuringElement(
        cv::MORPH_RECT, 
        cv::Size(3, 3));
}

