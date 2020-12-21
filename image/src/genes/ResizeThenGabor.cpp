#include "../../include/genes/ResizeThenGabor.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <cassert>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::ResizeThenGaborGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minSigma_ = std::stof(geneParameters.at("minSigma"));
    maxSigma_ = std::stof(geneParameters.at("maxSigma"));
    minTheta_ = std::stof(geneParameters.at("minTheta"));
    maxTheta_ = std::stof(geneParameters.at("maxTheta"));
    minLambda_ = std::stof(geneParameters.at("minLambda"));
    maxLambda_ = std::stof(geneParameters.at("maxLambda"));
    minGamma_ = std::stof(geneParameters.at("minGamma"));
    maxGamma_ = std::stof(geneParameters.at("maxGamma"));
    minScale_ = std::stoi(geneParameters.at("minScale"));
    maxScale_ = std::stoi(geneParameters.at("maxScale"));

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
        gamma_ = std::stof(geneParameters.at("gamma"));
    } else {
        mutateGamma();
    }

    if (geneParameters.count("scale") > 0) {
        scale_ = std::stoi(geneParameters.at("scale"));
    } else {
        mutateScale();
    }

    refreshResizeThenGaborKernel();
}

void ci::ResizeThenGaborGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::ResizeThenGaborGene* gene = dynamic_cast<const ci::ResizeThenGaborGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing ResizeThenGaborGene with gene of improper type '"
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
    refreshResizeThenGaborKernel(); 
}

void ci::ResizeThenGaborGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();

    if (f < 0.2) {
        mutateSigma();
    } else if (f < 0.4) {
        mutateTheta();
    } else if (f < 0.6) {
        mutateLambda();
    } else if (f < 0.8) {
        mutateGamma();
    } else {
        mutateScale();
    }

    if (f < 0.8) {
        refreshResizeThenGaborKernel();
    }
}

void ci::ResizeThenGaborGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::Mat tmp;
        cv::Mat tmp2;
        cv::resize(
            input,
            tmp,
            cv::Size((input.cols/scale_), (input.rows/scale_)),
            cv::INTER_AREA);

        cv::resize(
            tmp,
            tmp2,
            cv::Size(input.cols, input.rows),
            cv::INTER_LANCZOS4);


        cv::filter2d(tmp2, output, -1, gaborKernel_);
    }
}

std::string ci::ResizeThenGaborGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "  cv::Mat tmp;\n"
        << "  cv::Mat tmp2;\n"
        << "  \n"
        << "  cv::resize(\n"
        << "    $INPUT,\n"
        << "    tmp,\n"
        << "    cv::Size($INPUT.cols/$SCALE, $INPUT.rows/$SCALE)\n"
        << "    cv::INTER_AREA);\n"
        << "\n"
        << "  cv::resize(\n"
        << "    tmp,\n"
        << "    tmp2,\n"
        << "    cv::Size($INPUT.cols, $INPUT.rows)\n"
        << "    cv::INTER_LANCZOS4);\n"
        << "\n"
        << "  cv::Mat kernel = cv::getGaborKernel(\n"
        << "    cv::Size(3,3),\n"
        << "    " << std::to_string(sigma_) << ",\n"
        << "    " << std::to_string(theta_) << ",\n"
        << "    " << std::to_string(lambda_) << ",\n"
        << "    " << std::to_string(gamma_) << "\n"
        << "  );\n"
        << "\n"
        << "  cv::filter2d(tmp2, $OUTPUT, -1, kernel);\n"
        << "}\n";

    std::string output = replaceAllFxn(
        codeTemplate.str(),
        "$INPUT",
        context.inputVariableNames[0]);

    output = replaceAllFxn(
        output,
        "$OUTPUT",
        context.outputVariableName);

    output = replaceAllFxn(
        output,
        "$SCALE",
        std::to_string(scale_));


    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::ResizeThenGaborGene::serializeInternal() const {
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

    tmp["minScale"] = minScale_;
    tmp["maxScale"] = maxScale_;
    tmp["scale"] = scale_;

    return tmp;
}

void ci::ResizeThenGaborGene::refreshResizeThenGaborKernel() {
    // TODO: should we allow modification of kernel size?
    gaborKernel_ = cv::getResizeThenGaborKernel(
        cv::Size(3, 3),
        sigma_,
        theta_, 
        lambda_,
        gamma_);
}

void ci::ResizeThenGaborGene::mutateSigma() {
    float range = maxSigma_ - minSigma_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    sigma_ = minSigma_ + offset;
}

void ci::ResizeThenGaborGene::mutateTheta() {
    float range = maxTheta_ - minTheta_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    theta_ = minTheta_ + offset;
}

void ci::ResizeThenGaborGene::mutateLambda() {
    float range = maxLambda_ - minLambda_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    lambda_ = minLambda_ + offset;
}

void ci::ResizeThenGaborGene::mutateGamma() {
    float range = maxGamma_ - minGamma_;
    float offset = range * randomNumberGenerator_->getRandomFloat();
    gamma_ = minGamma_ + offset;
}

void ci::ResizeThenGaborGene::mutateScale() {
    scale_ = randomNumberGenerator_->getRandomInt(minScale_, maxScale_);
}
