#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <opencv2/core/core.hpp>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kUnsharpenGeneTypeId = 38;
static constexpr const char* kUnsharpenGeneTypeName = "unsharpen";

// Performs the Unsharpen operation on an input image
// Unsharpening is convolution with a kernel like [[-1, -1, -1], [-1, 9, -1], [-1, -1, -1]]
class UnsharpenGene : public cgpExperiments::core::Gene {
    public:
        virtual ~UnsharpenGene() {};
        virtual int getTypeId() const override { return kUnsharpenGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kUnsharpenGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 2130.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        cv::Mat kernel_;

        void initializeKernel();
};

}
}
