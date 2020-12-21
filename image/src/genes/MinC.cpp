#include "../../include/genes/MinC.hpp"

#include <opencv2/core/core.hpp>

#include <stdexcept>
#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::MinCGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    minValue_ = std::stoi(geneParameters.at("minValue"));
    maxValue_ = std::stoi(geneParameters.at("maxValue"));

    if (geneParameters.counnt("value") > 0) {
        value_ = std::stoi(geneParameters.at("value"));
    } else {
        mutateParameters();
    }
}

void ci::MinCGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    const ci::MinCGene* gene = dynamic_cast<const ci::MinCGene*>(other);
    if (gene == nullptr) {
        throw std::runtime_error(
            "Attempting to initialize MinCGene with a gene of type '"
            + other->getGeneName()
            + "'.");
    }

    minValue_ = gene->minValue_;
    maxValue_ = gene->maxValue_;
    value_ = gene->value_;
}

void ci::MinCGene::mutateParameters() {
    value_ = randomNumberGenerator_->getRandomInt(minValue_, maxValue_);
}

void ci::MinCGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        for (int y = 0; y < input.rows; y++) {
            for (int x = 0; x < input.cols; x++) {
                output.at<unsigned char>(y, x) = std::min(
                    input.at<unsigned char>(y, x),
                    value_);
            }
        }
    }
}

std::string ci::MinCGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "for (int y = 0; y < $INPUT.rows; y++) {\n"
        << "  for (int x = 0; x < $INPUT.cols; x++) {\n"
        << "    $OUTPUT.at<unsigned char>(y,x) = std::min(\n"
        << "      input.at<unsigned char>(y,x),\n"
        << "      $VAL);\n"
        << "  }\n"
        << "}\n";

    std::string output = replaceAllFxn(codeTemplate.str(), "$VAL", std::to_string(value_));
    output = replaceAllFxn(output, "$INPUT", context.inputVariableNames[0]);
    output = replaceAllFxn(output, "$OUTPUT", context.outputVariableName);

    return output;
}

std::unordered_map<std::string, std::string> ci::MinCGene::serializeInternal() const {
    std::unordered_map<std::string, std::string> tmp;

    tmp["minValue"] = std::to_string(minValue_);
    tmp["maxValue"] = std::to_string(maxValue_);
    tmp["value"] = std::to_string(value_);

    return tmp;
}
