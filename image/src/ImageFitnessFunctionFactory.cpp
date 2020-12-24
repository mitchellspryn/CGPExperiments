#include "../include/ImageFitnessFunctionFactory.hpp"
#include "../include/MCCFitnessFunction.hpp"
#include "../include/L1FitnessFunction.hpp"
#include "../include/L2FitnessFunction.hpp"

#include <stdexcept>
#include <strings.h>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

std::unique_ptr<cc::FitnessFunction> ci::ImageFitnessFunctionFactory::create(
        const std::unordered_map<std::string, std::string>& fitnessFunctionParameters) {

    if (fitnessFunctionParameters.count("name") == 0) { 
        throw std::runtime_error("Missing parameter 'name' in FitnessFunction definition.");
    }

    std::string fitnessFunctionName = fitnessFunctionParameters.at("name");
    const char* name = fitnessFunctionName.c_str();
    if (strncasecmp(name, ci::kMCCFitnessFunctionName, 4) == 0) {
        return std::make_unique<ci::MCCFitnessFunction>();
    } else if (strncasecmp(name, ci::kL1FitnessFunctionName, 4) == 0) {
        return std::make_unique<ci::L1FitnessFunction>();
    } else if (strncasecmp(name, ci::kL2FitnessFunctionName, 4) == 0) {
        return std::make_unique<ci::L2FitnessFunction>();
    }

    throw std::runtime_error(
        "Unrecognized fitness function name: '"
        + fitnessFunctionName
        + "'.");

}
