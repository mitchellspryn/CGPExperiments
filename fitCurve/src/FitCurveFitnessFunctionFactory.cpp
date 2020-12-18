#include "../include/FitCurveFitnessFunctionFactory.hpp"
#include "../include/L1ErrorFitnessFunction.hpp"
#include "../include/L2ErrorFitnessFunction.hpp"

#include <stdexcept>
#include <strings.h>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

std::unique_ptr<cc::FitnessFunction> fc::FitCurveFitnessFunctionFactory::create(
        const std::unordered_map<std::string, std::string>& fitnessFunctionParameters) {
    if (fitnessFunctionParameters.count("name") == 0) { 
        throw std::runtime_error("Missing parameter 'name' in FitnessFunction definition.");
    }

    std::string fitnessFunctionName = fitnessFunctionParameters.at("name");
    const char* name = fitnessFunctionName.c_str();
    if (strncasecmp(name, fc::kL1ErrorFitnessFunctionName, 3) == 0) {
        return std::make_unique<fc::L1ErrorFitnessFunction>();
    } else if (strncasecmp(name, fc::kL2ErrorFitnessFunctionName, 3) == 0) {
        return std::make_unique<fc::L2ErrorFitnessFunction>();
    }

    throw std::runtime_error(
        "Unrecognized fitness function name: '"
        + fitnessFunctionName
        + "'.");
}
