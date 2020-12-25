#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kSmoothMedianGeneTypeId = 30;
static constexpr const char* kSmoothMedianGeneTypeName = "smoothMedian";

// Performs median smoothing with a kernel of size [kernelSize_ x kernelSize_]
class SmoothMedianGene : public cgpExperiments::core::Gene {
    public:
        virtual ~SmoothMedianGene() {};
        virtual int getTypeId() const override { return kSmoothMedianGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kSmoothMedianGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }

        virtual float getComputeCost() const override { 
            switch (kernelSize_) {
                case 3:
                    return 823.0f;
                case 5:
                    return 5246.5f;
                case 7:
                    return 56734.0f;
                case 9:
                    return 64727.0f;
                case 11:
                    return 74116.5f;
                case 13:
                    return 83995.5f;
                case 15:
                    return 92649.5f;
                default:
                    throw std::runtime_error(
                        "Kernel size "
                        + std::to_string(kernelSize_)
                        + " not found in cost lookup table.");
            }
        }

        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minKernelSize_;
        int maxKernelSize_;
        int kernelSize_ = 3;
};

}
}
