#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace fitCurve {

class AddGene : public cgpExperiments::fitCurve::Gene<2> {
    public:
        virtual ~AddGene() {};
        virtual void initializeFromParameters(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const = 0;
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) override;
        virtual std::unordered_map<std::string, std::string> serializeInternal() override;
};

}
}
