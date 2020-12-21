#include "../../include/genes/Gabor.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cassert>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::GaborGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minSigma_ = std::stof(geneParameters.at("minSigma"));
    maxSigma_ = std::stof(geneParameters.at("maxSigma"));
    minTheta_ = std::stof(geneParameters.at("minTheta"));
    maxTheta_ = std::stof(geneParameters.at("maxTheta"));
    minLambda_ = std::stof(geneParameters.at("minLambda"));
    maxLambda_ = std::stof(geneParameters.at("maxLambda"));
    minGamma_ = std::stof(geneParameters.at("minGamma"));
    maxGamma_ = std::stof(geneParameters.at("maxGamma"));

    if (geneParameters.count("sigma") > 0) {
        sigma_ = std::stof(geneParameters.at("sigma"));
    } else {
        mutateSigma();
    }

    if (geneParameters.count("theta") > 0) {
        theta_ = std::stof(geneParameters.at("theta"));
    } else {
        mutateTheta();
    }

    if (geneParameters.count("lambda") > 0) {
        lambda_ = std::stof(geneParameters.at("lambda"));
    } else {
        mutateLambda();
    }

    if (geneParameters.count("gamma") > 0) {
        gamma_ = std::stof(geneParameters.at("gamma_"));
    } else {
        mutateGamma();
    }

    refreshGaborKernel();
}

void ci::GaborGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::GaborGene* gene = dynamic_cast<const ci::GaborGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing GaborGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minSigma_ = gene->minSigma_;
    maxSigma_ = gene->maxSigma_;
    sigma_ = gene->sigma_;

    minTheta_ = gene->minTheta_;
    maxTheta_ = gene->maxTheta_;
    theta_ = gene->theta_;

    minLambda_ = gene->minLambda_;
    maxLambda_ = gene->maxLambda_;
    lambda_ = gene->lambda_;

    minGamma_ = gene->minGamma_;
    maxGamma_ = gene->maxGamma_;
    gamma_ = gene->gamma_;

    // Recreate the kernel from scratch rather than risk shallow copy
    refreshGaborKernel(); 
}

void ci::GaborGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();

    if (f < 0.25) {
        mutateSigma();
    } else if (f < 0.50) {
        mutateTheta();
    } else if (f < 0.75) {
        mutateLambda();
    } else {
        mutateGamma();
    }

    refreshGaborKernel();
}

void ci::GaborGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::filter2d(input, output, -1, gaborKernel_);
    }
}

std::string ci::GaborGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;
    std::string tmpVariableName = generateUnusedVariableName(context);

    // TODO: this will hurt runtime efficiency if we get the kernel each time.
    //   Ideally, we should do it once and cache it in the class.
    //   However, that can be done manually for now.
    codeTemplate
        << "cv::Mat " 
        << tmpVariableName
        << " = cv::getGaborKernel(\n"
        << "  cv::Size(3,3),\n"
        << "  " << std::to_string(sigma_) << ",\n"
        << "  " << std::to_string(theta_) << ",\n"
        << "  " << std::to_string(lambda_) << ",\n"
        << "  " << std::to_string(gamma_) << "\n"
        << ");\n";

    codeTemplate
        << "cv::filter2d("
        << context.inputVariableNames[0]
        << ", "
        << context.outputVariableName
        << ", -1,"
        << tmpVariableName
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::GaborGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minSigma"] = minSigma_;
    tmp["maxSigma"] = maxSigma_;
    tmp["sigma"] = sigma_;

    tmp["minTheta"] = minTheta_;
    tmp["maxTheta"] = maxTheta_;
    tmp["theta"] = theta_;

    tmp["minLambda"] = minLambda_;
    tmp["maxLambda"] = maxLambda_;
    tmp["lambda"] = lambda_;

    tmp["minGamma"] = minGamma_;
    tmp["maxGamma"] = maxGamma_;
    tmp["gamma"] = gamma_;

    return tmp;
}

void ci::GaborGene::refreshGaborKernel() {
    // TODO: should we allow modification of kernel size?
    gaborKernel_ = cv::getGaborKernel(
        cv::Size(3, 3),
        sigma_,
        theta_, 
        lambda_,
        gamma_);
}

void ci::GaborGene::mutateSigma() {
    float range = maxSigma_ - minSigma_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    sigma_ = minSigma_ + offset;
}

void ci::GaborGene::mutateTheta() {
    float range = maxTheta_ - minTheta_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    theta_ = minTheta_ + offset;
}

void ci::GaborGene::mutateLambda() {
    float range = maxLambda_ - minLambda_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    lambda_ = minLambda_ + offset;
}

void ci::GaborGene::mutateGamma() {
    float range = maxGamma_ - minGamma_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    gamma_ = minGamma_ + offset;
}
