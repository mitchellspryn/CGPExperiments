#pragma once

#include <memory>
#include <string>

#include "ExperimentConfiguration.hpp"
#include "FitnessFunction.hpp"

namespace cgpExperiments {
namespace core {

class FitnessFunctionFactory {
    public:
        virtual ~FitnessFunctionFactory() {};
        virtual std::unique_ptr<FitnessFunction> create(const std::unordered_map<std::string, std::string>& fitnessFunctionParameters) = 0;
};

}
}
