#pragma once

#include "../../core/include/FitnessFunction.hpp"
#include "../../core/include/Genotype.hpp"

#include <string>
#include <unordered_map>

namespace cgpExperiments {
namespace image {

static constexpr const char* kMccPerfFitnessFunctionName = "mccPerf";

class MccPerfFitnessFunction : public cgpExperiments::core::FitnessFunction {
    public:
        MccPerfFitnessFunction(double lambda);
        MccPerfFitnessFunction(const std::unordered_map<std::string, std::string>& parameters);
        virtual ~MccPerfFitnessFunction() override {};
        virtual double evaluate(
            const cgpExperiments::core::DataChunk& predictions, 
            const cgpExperiments::core::DataChunk& labels, 
            const cgpExperiments::core::Genotype& genotype) override;

        double computeMccLoss(
            const cgpExperiments::core::DataChunk& predictions,
            const cgpExperiments::core::DataChunk& labels);

        double computePerfLoss(const cgpExperiments::core::Genotype& genotype);

    private:
        double lambda_ = 0;
};

}
}
