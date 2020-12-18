#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kLocalNormalizeGeneTypeId = 15;
static constexpr const char* kLocalNormalizeGeneTypeName = "localNormalize";

// Performs the normalization operation on each pixel on a window centered at (x, y)
// That is, output[x, y] = normalize(input[x-(width/2):x+(width/2), y-(height/2):y+(height/2)])
class LocalNormalizeGene : public cgpExperiments::core::Gene {
    public:
        virtual ~LocalNormalize() {};
        virtual int getTypeId() const override { return kLocalNormalizeGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kLocalNormalizeGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int neighborhoodWidth_;
        int neighborhoodHeight_;
};

}
}
