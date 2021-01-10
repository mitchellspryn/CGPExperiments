#pragma once

#include <string>
#include <unordered_set>

#include "DataChunk.hpp"
#include "Genotype.hpp"

namespace cgpExperiments {
namespace core {

// A class that can evaluate the fitness of a genotype.
class FitnessFunction {
    public:
        virtual ~FitnessFunction() {};

        // This method is used to evaluate the performance of a genotype.
        // The output should be on the range [0, infinity], where 0  is a perfect solution.
        virtual double evaluate(const DataChunk& predictions, const DataChunk& labels, const Genotype& genotype) = 0;
};

}
}
