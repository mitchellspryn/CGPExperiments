#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kGaussianBlurTypeId = 10;
static constexpr const char* kGaussianBlurGeneTypeName = "gaussianBlur";

// Performs a gaussain blur of [kernelSize x kernelSize] over the image
class GaussianBlurGene : public cgpExperiments::core::Gene {
    public:
        virtual ~GaussianBlurGene() {};
        virtual int getTypeId() const override { return kGaussianBlurGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kGaussianBlurGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int kernelSize_;
};

}
}
