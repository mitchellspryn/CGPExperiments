#pragma once

#include "../../core/include/FitnessFunction.hpp"
#include "../../core/include/Genotype.hpp"

namespace cgpExperiments {
namespace image {

static constexpr const char* kL1FitnessFunctionName = "l1";

class L1FitnessFunction : public cgpExperiments::core::FitnessFunction {
    public:
        virtual ~L1FitnessFunction() override {};
        virtual double evaluate(
            const cgpExperiments::core::DataChunk& predictions, 
            const cgpExperiments::core::DataChunk& labels, 
            const cgpExperiments::core::Genotype& genotype) override;
};

}
}
