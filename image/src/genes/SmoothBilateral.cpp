#include "../../include/genes/SmoothBilateral.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::SmoothBilateralGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minD_ = std::stoi(geneParameters.at("minD"));
    maxD_ = std::stoi(geneParameters.at("maxD"));
    minSigma_ = std::stoi(geneParameters.at("minSigma"));
    maxSigma_ = std::stoi(geneParameters.at("maxSigma"));

    if (geneParameters.count("d") > 0) {
        d_ = std::stoi(geneParameters.at("d"));
    } else {
        mutateD();
    }

    if (geneParameters.count("sigma") > 0) {
        sigma_ = std::stof(geneParameters.at("sigma"));
    } else {
        mutateSigma();
    }
}

void ci::SmoothBilateralGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::SmoothBilateralGene* gene = dynamic_cast<const ci::SmoothBilateralGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing SmoothBilateralGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minD_ = gene->minD_;
    maxD_ = gene->maxD_;
    d_ = gene->d_;

    minSigma_ = gene->minSigma_;
    maxSigma_ = gene->maxSigma_;
    sigma_ = gene->sigma_;
}


void ci::SmoothBilateralGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();
    if (f < 0.5) { 
        mutateD();
    } else {
        mutateSigma();
    }
}

void ci::SmoothBilateralGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::bilateralFilter(
            input, 
            output, 
            d_,
            sigma_,
            sigma_);
    }
}

std::string ci::SmoothBilateralGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::bilateralFilter("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", "
        << std::to_string(d_)
        << ", "
        << std::to_string(sigma_)
        << ", "
        << std::to_string(sigma_)
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::SmoothBilateralGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minD"] = std::to_string(minD_);
    tmp["maxD"] = std::to_string(maxD_);
    tmp["d"] = std::to_string(d_);

    tmp["minSigma"] = std::to_string(minSigma_);
    tmp["maxSigma"] = std::to_string(maxSigma_);
    tmp["sigma"] = std::to_string(sigma_);

    return tmp;
}

void ci::SmoothBilateralGene::mutateD() {
    d_ = randomNumberGenerator_->getRandomInt(minD_, maxD_);
}

void ci::SmoothBilateralGene::mutateSigma() {
    float range = maxSigma_ - minSigma_;
    float offset = randomNumberGenerator_->getRandomFloat() * range;
    sigma_ = minSigma_ + offset;
}

