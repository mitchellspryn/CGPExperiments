#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kSobelGeneTypeId = 31;
static constexpr const char* kSobelGeneTypeName = "sobel";

// Performs a horizontal and vertical sobel edge detection operator.
class SobelGene : public cgpExperiments::core::Gene {
    public:
        virtual ~SobelGene() {};
        virtual int getTypeId() const override { return kSobelGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kSobelGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 12521.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
