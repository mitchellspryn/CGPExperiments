#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kMultiplyGeneTypeId = 24;
static constexpr const char* kMultiplyGeneTypeName = "multiply";

// Computes the pixel-wise multiplication of two images.
// That is, output[x,y] = a[x,y] * b[x,y]
class MultiplyGene : public cgpExperiments::core::Gene {
    public:
        virtual ~MultiplyGene() {};
        virtual int getTypeId() const override { return kMultiplyGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kMultiplyGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 2; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
