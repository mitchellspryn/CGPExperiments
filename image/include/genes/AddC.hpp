#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kAddCGeneTypeId = 1;
static constexpr const char* kAddCGeneTypeName = "addC";

// Adds a constant to each pixel
class AddCGene : public cgpExperiments::core::Gene {
    public:
        virtual ~AddCGene() {};
        virtual int getTypeId() const override { return kAddCGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kAddCGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 790.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minValue_;
        int maxValue_;
        int value_;
};

}
}
