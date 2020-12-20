#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <strings.h>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "../core/include/CgpTrainer.hpp"
#include "../core/include/ExperimentConfiguration.hpp"
#include "../core/include/FitnessFunctionFactory.hpp"
#include "../core/include/GeneFactory.hpp"
#include "../core/include/GenePool.hpp"
#include "../core/include/RandomNumberGenerator.hpp"

#include "../fitCurve/include/FitCurveFitnessFunctionFactory.hpp"
#include "../fitCurve/include/FitCurveGeneFactory.hpp"

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

std::shared_ptr<cc::ExperimentConfiguration> getExperimentConfiguration(
        const std::string& jsonFileName) {
    return std::make_shared<cc::ExperimentConfiguration>(jsonFileName);
}

std::shared_ptr<cc::GeneFactory> createGeneFactory(
        const std::string& geneSet) {
    if (strncasecmp(geneSet.c_str(), "fitcurve", 9) == 0) {
        return std::make_shared<fc::FitCurveGeneFactory>();
    }

    throw std::runtime_error(
        "Unrecognized geneset: '"
        + geneSet
        + "'.");
}

std::shared_ptr<cc::RandomNumberGenerator> createRandomNumberGenerator() {
    return std::make_shared<cc::RandomNumberGenerator>(0);
}

std::shared_ptr<cc::GenePool> createGenePool(
        std::shared_ptr<cc::GeneFactory> geneFactory,
        std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator,
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration) {
    return std::make_shared<cc::GenePool>(
            experimentConfiguration,
            geneFactory,
            std::make_unique<cc::RandomNumberGenerator>(0));
}

std::vector<std::unordered_map<std::string, std::string>> loadParametersFromJson(
        const std::string fileName) {
    std::ifstream stream(fileName);
    std::stringstream buffer;
    buffer << stream.rdbuf();

    nlohmann::json j = nlohmann::json::parse(buffer.str());

    std::vector<std::unordered_map<std::string, std::string>> result;

    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
        std::unordered_map<std::string, std::string> tmp;
        nlohmann::json jobj = *it;
        
        for (nlohmann::json::iterator iit = jobj.begin(); iit != jobj.end(); ++iit) {
            std::string key = iit.key();
            if (iit.value().type() == nlohmann::json::value_t::string) {
                tmp[key] = iit.value().get<std::string>();
            } else {
                tmp[key] = iit.value().dump();
            }
        }

        result.push_back(tmp);
    }

    return result;
}

std::shared_ptr<cc::Genotype> createGenotype(
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration,
        std::shared_ptr<cc::GenePool> genePool,
        std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator,
        const std::vector<std::unordered_map<std::string, std::string>> params) {
    
    std::shared_ptr<cc::Genotype> genotype = 
        std::make_shared<cc::Genotype>(
            experimentConfiguration,
            genePool,
            randomNumberGenerator);

    genotype->deserialize(params);

    return genotype;
}

const cc::DataChunk& runPredictions(
        std::shared_ptr<cc::Genotype> genotype,
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration,
        std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator) {
    const std::unordered_map<std::string, std::string> genotypeParameters = 
        experimentConfiguration->getGenotypeParameters();

    const std::vector<std::unordered_map<std::string, std::string>> inputDataChunkParams = 
        experimentConfiguration->getInputDataChunkProviderParameters();

    int expectedNumDatasets = inputDataChunkParams.size();

    std::vector<std::shared_ptr<cc::DataChunkProvider>> providers;
    for (int i = 0; i < expectedNumDatasets; i++) {
        providers.emplace_back(
            std::make_shared<cc::DataChunkProvider>(
                inputDataChunkParams.at(i),
                randomNumberGenerator));
    }

    std::vector<std::shared_ptr<cc::DataChunk>> inputDataChunks;
    for (int i = 0; i < expectedNumDatasets; i++) {
        inputDataChunks.emplace_back(
            std::make_shared<cc::DataChunk>(
                std::stoi(genotypeParameters.at("inputDataWidth")),
                std::stoi(genotypeParameters.at("inputDataHeight")),
                std::stoi(genotypeParameters.at("inputDataNumSamples")),
                experimentConfiguration->getDataTypeSize()));

        providers[i]->getRandomChunk(*inputDataChunks[i], 0);
    }

    return genotype->evaluate(inputDataChunks);
}

void saveChunkToFile(
        const std::string& fileName,
        const cc::DataChunk& chunk) {
    std::ofstream stream(fileName, std::ios::out | std::ios::binary);

    const unsigned char* data = chunk.getConstCharDataPtr();
    int size = chunk.getSizeInBytes();

    stream.write(data, size);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: DumpBuffers <experimentConfig> <genotypeJson>\n";
        return EXIT_FAILURE;
    }

    std::string experimentConfigPath = std::string(argv[1]);
    std::string genotypeJsonPath = std::string(argv[2]);

    std::cout << "Initializing containers..." << std::endl;
    std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration
        = getExperimentConfiguration(experimentConfigPath);

    const std::string& geneSet = experimentConfiguration->getGeneset();
    std::shared_ptr<cc::GeneFactory> geneFactory =
        createGeneFactory(geneSet);

    std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator = 
        createRandomNumberGenerator();

    std::shared_ptr<cc::GenePool> genePool = 
        createGenePool(geneFactory, randomNumberGenerator, experimentConfiguration);

    std::cout << "Loading genotype..." << std::endl;
    std::vector<std::unordered_map<std::string, std::string>> genotypeParams = 
        loadParametersFromJson(genotypeJsonPath);

    std::cout << "Creating genotype..." << std::endl;
    std::shared_ptr<cc::Genotype> genotype = createGenotype(
        experimentConfiguration,
        genePool,
        randomNumberGenerator,
        genotypeParams);

    std::cout << "Running prediction..." << std::endl;
    const cc::DataChunk& prediction = 
        runPredictions(genotype, experimentConfiguration, randomNumberGenerator);

    std::cout << "Dumping buffers..." << std::endl;
    const std::vector<std::shared_ptr<cc::DataChunk>> buffers = 
        genotype->getDataBuffers();

    const std::string folderName = "dump/";
    const std::string extension = ".buf";
    std::filesystem::create_directory(folderName);

    for (size_t i = 0; i < buffers.size(); i++) {
        saveChunkToFile(
            folderName + std::to_string(i) + extension,
            *buffers[i]);
    }

    saveChunkToFile(folderName + "predictions" + extension, prediction);

    std::cout << "Graceful termination." << std::endl;
    return EXIT_SUCCESS;
}
