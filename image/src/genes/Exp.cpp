#include "../../include/genes/Exp.hpp"

#include <opencv2/core/core.hpp>

#include <sstream>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

void ci::ExpGene::initializeParametersFromConfig(
        const std::unordered_map<std::string, std::string>& geneParameters) {
    // Nothing to initialize!
}

void ci::ExpGene::initializeParametersFromTemplateGene(const cc::Gene* other) {
    // Nothing to initialize!
}

void ci::ExpGene::mutateParameters() {
    // Nothing to mutate!
}

void ci::ExpGene::evaluate(std::vector<std::shared_ptr<cc::DataChunk>>& buffers) {
    int width = buffers[0]->getWidth();
    int height = buffers[0]->getHeight();
    int num = buffers[0]->getNum();

    unsigned char* inputData = buffers[inputBufferIndices_[0]]->getCharDataPtr();
    unsigned char* outputData = buffers[outputBufferIndex_]->getCharDataPtr();

    for (int i = 0; i < num; i++) {
        int offset = (width*height*i);

        cv::Mat input(height, width, CV_8UC1, inputData + offset);
        cv::Mat output(height, width, CV_8UC1, outputData + offset);

        // For exp operator, needs to be CV_32FC1
        cv::Mat tmp;
        input.convertTo(tmp, CV_32F);
        cv::exp(tmp, tmp);
        tmp.convertTo(output, CV_8U);
    }
}

std::string ci::ExpGene::generateCode(cc::CodeGenerationContext_t& context) const {
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
        << "  $INPUT.convertTo(tmp, CV_32F);\n"
        << "  cv::exp(tmp, tmp);\n"
        << "  tmp.convertTo($OUTPUT, CV_8U);\n"
        << "}\n";

    std::string output = codeTemplate.str();
    output = replaceAllFxn(output, "$OUTPUT", context.outputVariableName);
    output = replaceAllFxn(output, "$INPUT", context.inputVariableNames[0]);

    return output;
}

std::unordered_map<std::string, std::string> ci::ExpGene::serializeInternal() const {
    // No parameters to serialize!
    std::unordered_map<std::string, std::string> tmp;
    return tmp;
}
