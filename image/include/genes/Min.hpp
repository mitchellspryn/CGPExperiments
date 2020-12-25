#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kMinGeneTypeId = 21;
static constexpr const char* kMinGeneTypeName = "min";

// Computes the pixel-wise minimum of two input images. 
// That is, output[x, y] = min(a[x,y], b[x,y])
class MinGene : public cgpExperiments::core::Gene {
    public:
        virtual ~MinGene() {};
        virtual int getTypeId() const override { return kMinGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kMinGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 2; }
        virtual float getComputeCost() const override { return 181.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
