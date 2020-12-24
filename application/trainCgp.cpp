#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <strings.h>

#include "../core/include/CgpTrainer.hpp"
#include "../core/include/ExperimentConfiguration.hpp"
#include "../core/include/FitnessFunctionFactory.hpp"
#include "../core/include/GeneFactory.hpp"

#include "../fitCurve/include/FitCurveFitnessFunctionFactory.hpp"
#include "../fitCurve/include/FitCurveGeneFactory.hpp"

#include "../image/include/ImageFitnessFunctionFactory.hpp"
#include "../image/include/ImageGeneFactory.hpp"

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;
namespace ci = cgpExperiments::image;

std::shared_ptr<cc::ExperimentConfiguration> getExperimentConfiguration(
        const std::string& jsonFileName) {
    return std::make_shared<cc::ExperimentConfiguration>(jsonFileName);
}

std::shared_ptr<cc::GeneFactory> createGeneFactory(
        const std::string& geneSet) {
    if (strncasecmp(geneSet.c_str(), "fitcurve", 9) == 0) {
        return std::make_shared<fc::FitCurveGeneFactory>();
    } else if (strncasecmp(geneSet.c_str(), "image", 9) == 0) {
        return std::make_shared<ci::ImageGeneFactory>();
    }

    throw std::runtime_error(
        "Unrecognized geneset: '"
        + geneSet
        + "'.");
}

std::shared_ptr<cc::FitnessFunctionFactory> createFitnessFunctionFactory(
        const std::string& geneSet) {
    if (strncasecmp(geneSet.c_str(), "fitcurve", 9) == 0) {
        return std::make_shared<fc::FitCurveFitnessFunctionFactory>();
    } else if (strncasecmp(geneSet.c_str(), "image", 9) == 0) {
        return std::make_shared<ci::ImageFitnessFunctionFactory>();
    }

    throw std::runtime_error(
        "Unrecognized geneset: '"
        + geneSet
        + "'.");
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./trainCgp <experiment_config_file>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string jsonConfigFilePath(argv[1]);

    std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration = 
        getExperimentConfiguration(jsonConfigFilePath);

    const std::string& geneSet = experimentConfiguration->getGeneset();
    std::shared_ptr<cc::GeneFactory> geneFactory =
        createGeneFactory(geneSet);

    std::shared_ptr<cc::FitnessFunctionFactory> fitnessFunctionFactory = 
        createFitnessFunctionFactory(geneSet);

    cc::CgpTrainer trainer(
        fitnessFunctionFactory,
        geneFactory,
        experimentConfiguration);

    trainer.run();

    std::cout << "Best fitness: " << trainer.getBestGenotypeFitness() << std::endl;
    return EXIT_SUCCESS;
}
