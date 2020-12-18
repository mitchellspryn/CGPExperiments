#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kNormalizeGeneTypeId = 25;
static constexpr const char* kNormalizeGeneTypeName = "normalize";

// Globally computes the normalization operator for the gene.
// Normalization linearly re-scales all pixels so that the minimum value maps to 0, and the maximum to 255
class NormalizeGene : public cgpExperiments::core::Gene {
    public:
        virtual ~NormalizeGene() {};
        virtual int getTypeId() const override { return kNormalizeGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kNormalizeGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
