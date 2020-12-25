#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace fitCurve {

static constexpr int kConstantOutputGeneTypeId = 1;
static constexpr const char* kConstantOutputGeneTypeName = "constantOutput";

class ConstantOutputGene : public cgpExperiments::core::Gene {
    public:
        virtual ~ConstantOutputGene() {};
        virtual int getTypeId() const override { return kConstantOutputGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kConstantOutputGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 0; }
        virtual float getComputeCost() const override { return 0.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        float minValue_;
        float maxValue_;
        float value_;
};

}
}
