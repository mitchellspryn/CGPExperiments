#pragma once

#include <memory>
#include <string>

#include "ExperimentConfiguration.hpp"
#include "FitnessFunction.hpp"

namespace cgpExperiments {
namespace core {

// A class that can generate a fitness function for use in the CGP training process.
class FitnessFunctionFactory {
    public:
        virtual ~FitnessFunctionFactory() {};

        // Creates the fitness function.
        // Override this in your geneset to construct the correct fitness function during training.
        // The parameters are the same as "fitnessFunctionParameters" in the configuration json.
        virtual std::unique_ptr<FitnessFunction> create(const std::unordered_map<std::string, std::string>& fitnessFunctionParameters) = 0;
};

}
}
