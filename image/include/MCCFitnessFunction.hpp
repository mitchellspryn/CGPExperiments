#pragma once

#include "../../core/include/FitnessFunction.hpp"
#include "../../core/include/Genotype.hpp"

namespace cgpExperiments {
namespace image {

static constexpr const char* kMCCFitnessFunctionName = "mcc";

class MCCFitnessFunction : public cgpExperiments::core::FitnessFunction {
    public:
        virtual ~MCCFitnessFunction() override {};
        virtual double evaluate(
            const cgpExperiments::core::DataChunk& predictions, 
            const cgpExperiments::core::DataChunk& labels, 
            const cgpExperiments::core::Genotype& genotype) override;
};

}
}
