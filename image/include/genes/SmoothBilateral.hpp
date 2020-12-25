#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kSmoothBilateralGeneTypeId = 29;
static constexpr const char* kSmoothBilateralGeneTypeName = "smoothBilateral";

// Performs bilateral smoothing
class SmoothBilateralGene : public cgpExperiments::core::Gene {
    public:
        virtual ~SmoothBilateralGene() {};
        virtual int getTypeId() const override { return kSmoothBilateralGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kSmoothBilateralGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override {
            switch (d_) {
                case 3:
                    return 18729.0f;
                case 4:
                case 5:
                case 6:
                case 7:
                    return 48000.0f;
                case 8:
                case 9:
                    return 73273.0f;
                case 10:
                    return 118757.5f;
                case 11:
                case 12:
                case 13:
                    return 163887.5f;
                case 14:
                    return 217610.0f;
                case 15:
                case 16:
                    return 287914.0f;
                default:
                    throw std::runtime_error(
                        "D size "
                        + std::to_string(d_)
                        + " not found in cost lookup table.");
            }
        }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minD_;
        int maxD_;
        int d_ = 3;

        float minSigma_;
        float maxSigma_;
        float sigma_;

        void mutateD();
        void mutateSigma();
};

}
}
