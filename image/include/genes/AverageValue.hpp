#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kAverageValueGeneTypeId = 4;
static constexpr const char* kAverageValueGeneTypeName = "averageValue";

// Finds the average value in a image, and outputs a constant image with every pixel set to that value
class AverageValueGene : public cgpExperiments::core::Gene {
    public:
        virtual ~AverageValueGene() {};
        virtual int getTypeId() const override { return kAverageValueGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kAverageValueGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 1640.5f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
