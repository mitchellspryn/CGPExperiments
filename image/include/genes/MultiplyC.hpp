#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kMultiplyCGeneTypeId = 23;
static constexpr const char* kMultiplyCGeneTypeName = "multiplyC";

// Multiplies each pixel with a constant value.
// That is, output[x, y] = C * input[x, y]
class MultiplyCGene : public cgpExperiments::core::Gene {
    public:
        virtual ~MultiplyCGene() {};
        virtual int getTypeId() const override { return kMultiplyCGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kMultiplyCGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        float value_;
};

}
}
