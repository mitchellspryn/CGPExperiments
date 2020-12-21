#include "../../include/genes/Shift.hpp"

#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::ShiftGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minVerticalShift_ = std::stoi(geneParameters.at("minVerticalShift"));
    maxVerticalShift_ = std::stoi(geneParameters.at("maxVerticalShift"));
    minHorizontalShift_ = std::stoi(geneParameters.at("minHorizontalShift"));
    maxHorizontalShift_ = std::stoi(geneParameters.at("maxHorizontalShift"));

    if (geneParameters.count("verticalShift") > 0) {
        verticalShift_ = std::stoi(geneParameters.at("verticalShift"));
    } else {
        mutateVerticalShift();
    }

    if (geneParameters.count("horizontalShift") > 0) {
        horizontalShift_ = std::stoi(geneParameters.at("horizontalShift"));
    } else {
        mutateHorizontalShift();
    }

    refreshAffineMat();
}

void ci::ShiftGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
    const ci::ShiftGene* gene = dynamic_cast<const ci::ShiftGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Initializing ShiftGene with gene of improper type '"
            + other->getGeneName()
            + "'.");
    }

    refreshAffineMat();
}

void ci::ShiftGene::mutateParameters() {
    float f = randomNumberGenerator_->getRandomFloat();
    
    if (f < 0.5) {
        mutateVerticalShift();
    } else {
        mutateHorizontalShift();
    }

    refreshAffineMat();
}

void ci::ShiftGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        cv::warpAffine(
            input,
            output,
            affineMat_,
            cv::Size(input.cols, input.rows));
    }
}

std::string ci::ShiftGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "  double affineData[6] = {1, 0, $HS, 0, 1, $VS};\n"
        << "  cv::Mat affineMat(2, 3, CV_64FC1, affineData.data());\n"
        << "  cv::warpAffine(\n"
        << "    $INPUT,\n" 
        << "    $OUTPUT,\n"
        << "    affineMat,\n"
        << "    cv::Size($INPUT.cols, $INPUT.rows));\n"
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
        "$HS",
        std::to_string(horizontalShift_));

    output = replaceAllFxn(
        output,
        "$VS",
        std::to_string(verticalShift_));

    return output;
}

std::unordered_map<std::string, std::string> ci::ShiftGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minVerticalShift"] = std::to_string(minVerticalShift_);
    tmp["maxVerticalShift"] = std::to_string(maxVerticalShift_);
    tmp["verticalShift"] = std::to_string(verticalShift_);

    tmp["minHorizontalShift"] = std::to_string(minHorizontalShift_);
    tmp["maxHorizontalShift"] = std::to_string(maxHorizontalShift_);
    tmp["horizontalShift"] = std::to_string(horizontalShift_);

    return tmp;
}

void ci::ShiftGene::mutateVerticalShift() {
    verticalShift_ = randomNumberGenerator_->getRandomInt(
        minVerticalShift_,
        maxVerticalShift_);
}

void ci::ShiftGene::mutateHorizontalShift() {
    horizontalShift_ = randomNumberGenerator_->getRandomInt(
        minHorizontalShift_,
        maxHorizontalShift_);
}

void ci::ShiftGene::refreshAffineMat() {
    affineMat_ = cv::Mat::zeros(2, 3, CV_64FC1);
    affineMat_.at<double>(0, 0) = 1;
    affineMat_.at<double>(1, 1) = 1;
    affineMat_.at<double>(0, 2) = horizontalShift_;
    affineMat_.at<double>(1, 2) = verticalShift_;
}

