#pragma once

#include <string>
#include <unordered_set>

#include "DataChunk.hpp"
#include "Genotype.hpp"

namespace cgpExperiments {
namespace core {

class FitnessFunction {
    public:
        FitnessFunction(const std::unordered_set<std::string, std::string>& fitnessFunctionParameters) {};
        virtual ~FitnessFunction() {};
        virtual double evaluate(const DataChunk& predictions, const DataChunk& labels, const Genotype& genotype) = 0;
};

}
}
