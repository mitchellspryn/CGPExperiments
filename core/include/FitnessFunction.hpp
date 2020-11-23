#pragma once

#include <string>
#include <unordered_set>

#include "DataChunk.hpp"

namespace cgpExperiments {
namespace core {

class FitnessFunction {
    public:
        FitnessFunction(const std::unordered_set<std::string, std::string>& fitnessFunctionParameters) {};
        virtual ~FitnessFunction() {};
        virtual void evaluate(const DataChunk& predictions, const DataChunk& labels) = 0;
};

}
}
