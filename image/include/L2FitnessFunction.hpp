#pragma once

#include "../../core/include/FitnessFunction.hpp"
#include "../../core/include/Genotype.hpp"

namespace cgpExperiments {
namespace image {

static constexpr const char* kL2FitnessFunctionName = "l2";

class L2FitnessFunction : public cgpExperiments::core::FitnessFunction {
    public:
        virtual ~L2FitnessFunction() override {};
        virtual double evaluate(
            const cgpExperiments::core::DataChunk& predictions, 
            const cgpExperiments::core::DataChunk& labels, 
            const cgpExperiments::core::Genotype& genotype) override;
};

}
}
