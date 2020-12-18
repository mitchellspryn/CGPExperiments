#pragma once

#include "../../core/include/FitnessFunction.hpp"
#include "../../core/include/Genotype.hpp"

namespace cgpExperiments {
namespace fitCurve {

static constexpr const char* kL2ErrorFitnessFunctionName = "l2";

class L2ErrorFitnessFunction : public cgpExperiments::core::FitnessFunction {
    public:
        virtual ~L2ErrorFitnessFunction() override {};
        virtual double evaluate(
            const cgpExperiments::core::DataChunk& predictions, 
            const cgpExperiments::core::DataChunk& labels, 
            const cgpExperiments::core::Genotype& genotype) override;
};

}
}
