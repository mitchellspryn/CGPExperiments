#include "../include/ImageFitnessFunctionFactory.hpp"
#include "../include/MCCFitnessFunction.hpp"

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

std::unique_ptr<cc::FitnessFunction> ci::ImageFitnessFunctionFactory::create(
        const std::unordered_map<std::string, std::stirng& fitnessFunctionParameters) {

    if (fitnessFunctionParameters.count("name") == 0) { 
        throw std::runtime_error("Missing parameter 'name' in FitnessFunction definition.");
    }

    std::string fitnessFunctionName = fitnessFunctionParameters.at("name");
    const char* name = fitnessFunctionName.c_str();
    if (strncasecmp(name, kMCCFitnessFunctionName, 4) == 0) {
        return std::make_unique<fc::MCCFitnessFunction>();
    }

    throw std::runtime_error(
        "Unrecognized fitness function name: '"
        + fitnessFunctionName
        + "'.");

}
