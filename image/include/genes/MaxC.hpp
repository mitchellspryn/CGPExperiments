#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kMaxCGeneTypeId = 17;
static constexpr const char* kMaxCGeneTypeName = "maxC";

// Computes the maximum of a gene and a constant value.
// That is, output[x, y] = max(C, input[x, y])
class MaxCGene : public cgpExperiments::core::Gene {
    public:
        virtual ~MaxCGene() {};
        virtual int getTypeId() const override { return kMaxCGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kMaxCGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minValue_;
        int maxValue_;
        int value_;
};

}
}
