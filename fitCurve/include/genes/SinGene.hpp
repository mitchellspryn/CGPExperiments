#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace fitCurve {

static constexpr int kSinGeneTypeId = 5;
static constexpr const char* kSinGeneTypeName = "sin";

class SinGene : public cgpExperiments::core::Gene {
    public:
        virtual ~SinGene() {};
        virtual int getTypeId() const override { return kSinGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kSinGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 2.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
