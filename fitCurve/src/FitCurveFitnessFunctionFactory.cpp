#include "../include/FitCurveFitnessFunctionFactory.hpp"
#include "../include/L2ErrorFitnessFunction.hpp"

#include <stdexcept>
#include <strings.h>

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

std::unique_ptr<cc::FitnessFunction> cf::FitCurveFitnessFunctionFactory::create(
        const std::unordered_map<std::string, std::string>& fitnessFunctionParameters) {
    if (fitnessFunctionParameters.count("name") == 0) { 
        throw std::runtime_error("Missing parameter 'name' in FitnessFunction definition.");
    }

    std::string fitnessFunctionName = fitnessFunctionParameters.at(name);
    const char* name = fitnessFunctionName.c_str();
    if (strncasecmp(name, "l2", 3) == 0) {
        return std::make_unique<cf::L2ErrorFitnessFunction>();
    }

    throw std::runtime_error(
        "Unrecognized fitness function name: '"
        + fitnessFunctionName
        + "'.");
}
