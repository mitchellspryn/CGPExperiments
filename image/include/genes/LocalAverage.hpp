#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../../core/include/Gene.hpp"

namespace cgpExperiments {
namespace image {

static constexpr int kLocalAverageGeneTypeId = 12;
static constexpr const char* kLocalAverageGeneTypeName = "localAverage";

// For each pixel [x, y], compute the local average over a neighborhood of [width, height] pixels. 
// That is, output[x,y] = average(input[x-(width/2):x+(width/2), y-(height/2):y+(height/2)])
class LocalAverageGene : public cgpExperiments::core::Gene {
    public:
        virtual ~LocalAverageGene() {};
        virtual int getTypeId() const override { return kLocalAverageGeneTypeId; }
        virtual void initializeParametersFromConfig(const std::unordered_map<std::string, std::string>& geneParameters) override;
        virtual void initializeParametersFromTemplateGene(const cgpExperiments::core::Gene* other) override;
        virtual void mutateParameters() override;
        virtual std::string getGeneName() const override { return std::string(kLocalAverageGeneTypeName); }
        virtual void evaluate(std::vector<std::shared_ptr<cgpExperiments::core::DataChunk>>& buffers) override;
        virtual std::string generateCode(cgpExperiments::core::CodeGenerationContext_t& context) const override;
        virtual bool isParameterFree() const override { return false; }
        virtual int getNumInputs() const override { return 1; }
        virtual std::unordered_map<std::string, std::string> serializeInternal() const override;
    private:
        int minNeighborhoodWidth_;
        int maxNeighborhoodWidth_;
        int neighborhoodWidth_;

        int minNeighborhoodHeight_;
        int maxNeighborhoodHeight_;
        int neighborhoodHeight_;

        void mutateNeighborhoodWidth();
        void mutateNeighborhoodHeight();
};

}
}
