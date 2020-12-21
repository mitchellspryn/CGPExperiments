#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <opencv2/core/core.hpp>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kShiftGeneTypeId = 28;
static constexpr const char* kShiftGeneTypeName = "shift";

// Shifts the gene verticalShift_ pixels downward and horizontalShift_ pixels to the right.
// This function zero pads the shifted pixels.
class ShiftGene : public cgpExperiments::core::Gene {
    public:
        virtual ~ShiftGene() {};
        virtual int getTypeId() const override { return kShiftGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kShiftGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minVerticalShift_;
        int maxVerticalShift_;
        int verticalShift_;

        int minHorizontalShift_;
        int maxHorizontalShift_;
        int horizontalShift_;

        cv::Mat affineMat_;

        void mutateVerticalShift();
        void mutateHorizontalShift();
        void refreshAffineMat();
};

}
}
