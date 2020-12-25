#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <opencv2/core/core.hpp>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kLocalMinGeneTypeId = 14;
static constexpr const char* kLocalMinGeneTypeName = "localMin";

// For each pixel [x, y], compute the local maximum over a neighborhood of [width, height] pixels. 
// That is, output[x,y] = minimum(input[x-(width/2):x+(width/2), y-(height/2):y+(height/2)])
class LocalMinGene : public cgpExperiments::core::Gene {
    public:
        virtual ~LocalMinGene() {};
        virtual int getTypeId() const override { return kLocalMinGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kLocalMinGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual float getComputeCost() const override { return 963.0f; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minNeighborhoodWidth_;
        int minNeighborhoodHeight_;
        int maxNeighborhoodWidth_;
        int maxNeighborhoodHeight_;
        int neighborhoodWidth_;
        int neighborhoodHeight_;

        cv::Mat kernel_;

        void mutateNeighborhoodWidth();
        void mutateNeighborhoodHeight();

        void refreshKernel();
};

}
}
