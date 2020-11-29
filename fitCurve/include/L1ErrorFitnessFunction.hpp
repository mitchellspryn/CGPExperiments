#pragma once

#include "../../core/include/FitnessFunction.hpp"
#include "../../core/include/Genotype.hpp"

namespace cgpExperiments {
namespace fitCurve {

class L1ErrorFitnessFunction : public cgpExperiments::core::FitnessFunction {
    public:
        virtual ~L1ErrorFitnessFunction() override {};
        virtual double evaluate(
            const cgpExperiments::core::DataChunk& predictions, 
            const cgpExperiments::core::DataChunk& labels, 
            const cgpExperiments::core::Genotype& genotype) override;
};

}
}
