#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include <strings.h>

#include "../core/include/CgpTrainer.hpp"
#include "../core/include/ExperimentConfiguration.hpp"
#include "../core/include/FitnessFunctionFactory.hpp"
#include "../core/include/GeneFactory.hpp"
#include "../core/include/RandomNumberGenerator.hpp"

#include "../fitCurve/include/FitCurveFitnessFunctionFactory.hpp"
#include "../fitCurve/include/FitCurveGeneFactory.hpp"

#include "../image/include/ImageFitnessFunctionFactory.hpp"
#include "../image/include/ImageGeneFactory.hpp"

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;
namespace ci = cgpExperiments::image;

static constexpr int rngSeed = 42;

typedef struct iterationParams {
    int numIterations;
    int numMutateIterations;
    int numWarmupIterations;
} iterationParams_t;

std::shared_ptr<cc::RandomNumberGenerator> createRandomNumberGenerator() {
    return std::make_shared<cc::RandomNumberGenerator>(rngSeed);
}

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

std::string toJson(const std::unordered_map<std::string, std::string> params) {
    nlohmann::json j;
    for (const auto& kvp : params) {
        j[kvp.first] = kvp.second;
    }

    return j.dump();
}

std::vector<std::shared_ptr<cc::DataChunk>> initializeBuffers(
        std::shared_ptr<cc::ExperimentConfiguration> config,
        std::shared_ptr<cc::GeneFactory> geneFactory) {
    int numBuffersNeeded = 0;

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> allGeneParameters = 
        config->getAllGeneParameters();

    std::shared_ptr<cc::RandomNumberGenerator> rng = createRandomNumberGenerator();
    for (const auto& kvp : allGeneParameters) {
        const std::string geneName = kvp.first;
        const std::unordered_map<std::string, std::string> geneParameters = kvp.second;

        std::shared_ptr<cc::Gene> gene = geneFactory->createGene(
            geneFactory->getTypeId(geneName));

        gene->setRandomNumberGenerator(rng.get());

        gene->initializeFromParameters(geneParameters);

        numBuffersNeeded = std::max(numBuffersNeeded, gene->getNumInputs() + 1);
    }

    std::unordered_map<std::string, std::string> genotypeParams = 
        config->getGenotypeParameters();

    std::vector<std::unordered_map<std::string, std::string>> dataChunkParams = 
        config->getInputDataChunkProviderParameters();

    std::vector<std::shared_ptr<cc::DataChunkProvider>> providers;
    for (size_t i = 0; i < dataChunkParams.size(); i++) {
        providers.emplace_back(
            std::make_shared<cc::DataChunkProvider>(
                dataChunkParams[i],
                rng));
    }

    std::vector<std::shared_ptr<cc::DataChunk>> buffers;
    for (int i = 0; i < numBuffersNeeded; i++) {
        buffers.emplace_back(
            std::make_shared<cc::DataChunk>(
                std::stoi(genotypeParams["inputDataWidth"]),
                std::stoi(genotypeParams["inputDataHeight"]),
                std::stoi(genotypeParams["inputDataNumSamples"]),
                config->getDataTypeSize()));

        providers[(i%providers.size())]->getRandomChunk(
            *(buffers[buffers.size() - 1]),
            0);
    }

    return buffers;
}

std::shared_ptr<cc::Gene> primeGeneForBenchmarking(
        const std::string& geneName,
        std::shared_ptr<cc::ExperimentConfiguration> config,
        std::shared_ptr<cc::GeneFactory> geneFactory,
        std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator) {
    
    std::shared_ptr<cc::Gene> gene = geneFactory->createGene(
        geneFactory->getTypeId(geneName));

    randomNumberGenerator->seed(rngSeed);
    gene->setRandomNumberGenerator(randomNumberGenerator.get());

    gene->initializeFromParameters(config->getGeneParameters(geneName));

    for (int i = 0; i < gene->getNumInputs(); i++) {
        gene->connectInput(i, i);
    }
    
    gene->setOutputIndex(gene->getNumInputs());

    return gene;
}

void benchmarkGene(
        std::shared_ptr<cc::Gene> gene, 
        std::vector<std::shared_ptr<cc::DataChunk>>& buffers,
        const iterationParams_t iterationParams,
        std::vector<double>& runTimes,
        std::vector<std::string>& params) {

    std::chrono::high_resolution_clock clk;

    for (int i = 0; i < iterationParams.numWarmupIterations; i++) {
        gene->evaluate(buffers);
    }

    runTimes.clear();
    runTimes.reserve(iterationParams.numIterations);
    params.clear();
    params.reserve(iterationParams.numIterations);

    for (int i = 1; i <= iterationParams.numIterations; i++) {
        if ((i % iterationParams.numMutateIterations) == 0) {
            gene->mutateParameters();
        }

        std::chrono::high_resolution_clock::time_point start = clk.now();
        gene->evaluate(buffers);
        std::chrono::high_resolution_clock::time_point end = clk.now();

        runTimes.push_back(
            std::chrono::duration_cast<std::chrono::microseconds>(end-start).count());

        params.push_back(
            toJson(gene->serialize()));
    }
}
    

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cout << "Usage: ./GenePerfTest <experiment_config_file> <num_iterations> <num_mutate_iterations> <num_warmup_iterations> <output_filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << std::setprecision(3);
    std::cout << "Initializing...\n";

    std::string jsonConfigFilePath(argv[1]);

    iterationParams_t iterationParams;
    iterationParams.numIterations = std::stoi(std::string(argv[2]));
    iterationParams.numMutateIterations = std::stoi(std::string(argv[3]));
    iterationParams.numWarmupIterations = std::stoi(std::string(argv[4]));

    std::string outputFileName = std::string(argv[5]);

    std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration = 
        getExperimentConfiguration(jsonConfigFilePath);

    const std::string& geneSet = experimentConfiguration->getGeneset();
    std::shared_ptr<cc::GeneFactory> geneFactory =
        createGeneFactory(geneSet);

    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> allGeneParameters = 
        experimentConfiguration->getAllGeneParameters();

    std::vector<std::shared_ptr<cc::DataChunk>> buffers = 
        initializeBuffers(experimentConfiguration, geneFactory);

    std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator =
        createRandomNumberGenerator();

    std::ofstream outputStream(outputFileName, std::ios::out);
    outputStream << "geneName|iterationNum|runtime|params\n";

    int numGenes = allGeneParameters.size();
    int genesProcessed = 0;

    std::vector<double> runtimes;
    std::vector<std::string> params;
    for (auto& kvp : allGeneParameters) {
        genesProcessed++;
        double percentComplete = (100.0 * genesProcessed) / static_cast<double>(numGenes);
        std::cout 
            << "Processing gene "
            << genesProcessed
            << " / "
            << numGenes
            << " ("
            << percentComplete
            << " %)...";

        std::string geneName = kvp.first;

        std::shared_ptr<cc::Gene> gene = primeGeneForBenchmarking(
            geneName,
            experimentConfiguration,
            geneFactory,
            randomNumberGenerator);

        benchmarkGene(gene, buffers, iterationParams, runtimes, params);

        for (size_t i = 0; i < runtimes.size(); i++) {
            outputStream 
                << gene->getGeneName()
                << "|"
                << i
                << "|"
                << runtimes[i]
                << "|"
                << params[i]
                << "\n";
        }

        std::cout << "Done!" << std::endl;
    }

    std::cout << "All genes processed." << std::endl;
    return EXIT_SUCCESS;
}
