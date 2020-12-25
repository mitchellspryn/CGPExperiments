#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kSubtractCGeneTypeId = 35;
static constexpr const char* kSubtractCGeneTypeName = "subtractC";

// Subtracts a constant value from each pixel
// That is, output[x, y] = input[x, y] - C
class SubtractCGene : public cgpExperiments::core::Gene {
    public:
        virtual ~SubtractCGene() {};
        virtual int getTypeId() const override { return kSubtractCGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kSubtractCGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 183.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minValue_;
        int maxValue_;
        int value_;
};

}
}
