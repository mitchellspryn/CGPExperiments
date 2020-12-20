#include "../../include/genes/GaussianBlur.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::GaussianBlurGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minX_ = std::stoi(geneParameters.at("minX"));
    maxX_ = std::stoi(geneParameters.at("maxX"));
    minY_ = std::stoi(geneParameters.at("minY"));
    maxY_ = std::stoi(geneParameters.at("maxY"));

    if (geneParameters.count("x") > 0) {
        x_ = std::stoi(geneParameters.at("x")); 
    } else {
        mutateX();
    }

    if (geneParameters.count("y") > 0) {
        y_ = std::stoi(geneParameters.at("y"));
    } else {
        mutateY();
    }
}

void ci::GaussianBlurGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::GaussianBlurGene* gene = dynamic_cast<const ci::GaussianBlurGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing GaussianBlurGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    minX_ = gene->minX_;
    maxX_ = gene->maxX_;
    minY_ = gene->minY_;
    maxY_ = gene->maxY_;
    x_ = gene->x_;
    y_ = gene->y_;
}

void ci::GaussianBlurGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();
    if (f < 0.5) {
        mutateX();
    } else {
        mutateY();
    }
}

void ci::GaussianBlurGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::GaussianBlur(
            input, 
            output, 
            cv::Size(x_, y_), 
            0); // 0 == compute sigma from kernel size
    }
}

std::string ci::GaussianBlurGene::generateCode(cc::CodeGenerationContext_t& context) const {
    std::stringstream codeTemplate;

    codeTemplate
        << "cv::GaussianBlur("
        << context.inputVariableNames[0]
        << ","
        << context.outputVariableName
        << ", cv::Size(" << x_ << ", " << y_ << "), 0"
        << ");\n";

    return codeTemplate.str();
}

std::unordered_map<std::string, std::string> ci::GaussianBlurGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minX"] = std::to_string(minX_);
    tmp["maxX"] = std::to_string(maxX_);
    tmp["x"] = std::to_string(x_);

    tmp["minY"] = std::to_string(minY_);
    tmp["maxY"] = std::to_string(maxY_);
    tmp["y"] = std::to_string(y_);

    return tmp;
}

void ci::GaussianBlurGene::mutateX() {
    // x_ must be an odd, positive integer
    x_ = std::abs(randomNumberGenerator_->getRandomInt(minX_, maxX_));

    if ((x_ % 2) == 0) {
        float f = randomNumberGenerator_->getRandomFloat();
        if (f > 0.5) {
            x_ += 1;
        } else {
            x_ -= 1;
        }
    }
}

void ci::GaussianBlurGene::mutateY() {
    // y_ must be an odd, positive integer
    y_ = std::abs(randomNumberGenerator_->getRandomInt(minY_, maxY_));

    if ((y_ % 2) == 0) {
        float f = randomNumberGenerator_->getRandomFloat();
        if (f > 0.5) {
            y_ += 1;
        } else {
            y_ -= 1;
        }
    }
}

