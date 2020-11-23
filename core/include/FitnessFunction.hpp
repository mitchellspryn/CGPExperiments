#pragma once

#include <nlohmann/json.hpp>

#include "DataChunk.hpp"

namespace cgpExperiments {
namespace core {

class FitnessFunction {
    public:
        FitnessFunction(nlohmann::json::json parameters) {};
        virtual ~FitnessFunction() {};
        virtual void evaluate(const DataChunk& predictions, const DataChunk& labels) = 0;
};

}
}
