#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kSobelXGeneTypeId = 32;
static constexpr const char* kSobelXGeneTypeName = "sobelX";

// Performs the sobel operator in the X direction
class SobelXGene : public cgpExperiments::core::Gene {
    public:
        virtual ~SobelXGene() {};
        virtual int getTypeId() const override { return kSobelXGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kSobelXGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 12851.5f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
