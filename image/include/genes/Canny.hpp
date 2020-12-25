#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kCannyGeneTypeId = 5;
static constexpr const char* kCannyGeneTypeName = "canny";

// Runs the canny edge detector
class CannyGene : public cgpExperiments::core::Gene {
    public:
        virtual ~CannyGene() {};
        virtual int getTypeId() const override { return kCannyGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kCannyGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 3456.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;

    private:
        int minLowerThresh_;
        int maxLowerThresh_;
        int lowerThresh_;

        int minUpperThresh_;
        int maxUpperThresh_;
        int upperThresh_;

        inline void mutateLowerThreshold();
        inline void mutateUpperThreshold();
        inline void rectifyThresholds();
};

}
}
