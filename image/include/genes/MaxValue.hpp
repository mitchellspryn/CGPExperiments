#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kMaxValueGeneTypeId = 19;
static constexpr const char* kMaxValueGeneTypeName = "maxValue";

// Computes the maximum value in an image, then outputs a constant tensor of that value.
class MaxValueGene : public cgpExperiments::core::Gene {
    public:
        virtual ~MaxValueGene() {};
        virtual int getTypeId() const override { return kMaxValueGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kMaxValueGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
