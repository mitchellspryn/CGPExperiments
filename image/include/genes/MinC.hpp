#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kMinCGeneTypeId = 20;
static constexpr const char* kMinCGeneTypeName = "minC";

// Computes the pixel-wise minimum of an input and a constant.
// That is, output[x,y] = min(input[x,y], C)
class MinCGene : public cgpExperiments::core::Gene {
    public:
        virtual ~MinCGene() {};
        virtual int getTypeId() const override { return kMinCGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kMinCGeneTypeName); }
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
