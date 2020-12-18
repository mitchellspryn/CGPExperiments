#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace fitCurve {

static constexpr int kDivideProtectedGeneTypeId = 3;
static constexpr const char* kDivideProtectedGeneTypeName = "divideProtected";

class DivideProtectedGene : public cgpExperiments::core::Gene {
    public:
        virtual ~DivideProtectedGene() {};
        virtual int getTypeId() const override { return kDivideProtectedGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kDivideProtectedGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return true; }
        virtual int getNumInputs() const override { return 2; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
};

}
}
