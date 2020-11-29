#include "../include/CgpTrainer.hpp"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <strings.h>

namespace cc = cgpExperiments::core;

cc::CgpTrainer::CgpTrainer(
        std::shared_ptr<cc::FitnessFunctionFactory> fitnessFunctionFactory,
        std::shared_ptr<cc::GeneFactory> geneFactory,
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration) {
    experimentConfiguration_ = experimentConfiguration;
    fitnessFunctionFactory_ = fitnessFunctionFactory;

    fillParametersFromMap(experimentConfiguration_->getTrainerParameters());

    genePool_ = std::make_shared<cc::GenePool>(
        experimentConfiguration_,
        geneFactory,
        std::make_unique<cc::RandomNumberGenerator>(rngSeed_));

    rngSeed_++;

    std::vector<std::unordered_map<std::string, std::string>> inputDataChunkParameters =
        experimentConfiguration_->getInputDataChunkProviderParameters();
    inputDataChunkProviders_.reserve(inputDataChunkParameters.size());

    for (size_t i = 0; i < inputDataChunkParameters.size(); i++) {
        inputDataChunkProviders_.emplace_back(
            std::make_shared<cc::DataChunkProvider>(
                inputDataChunkParameters[i],
                std::make_shared<cc::RandomNumberGenerator>(rngSeed_)));
        
        rngSeed_++;
    }

    labelDataChunkProvider_ = 
        std::make_shared<cc::DataChunkProvider>(
            experimentConfiguration_->getLabelDataChunkProviderParameters(),
            std::make_shared<cc::RandomNumberGenerator>(rngSeed_));

    rngSeed_++;

    for (size_t i = 0; i < numIslands_; i++) {
        islands_.emplace_back(
            fitnessFunctionFactory_,
            genePool_,
            inputDataChunkProviders_,
            labelDataChunkProvider_,
            experimentConfiguration,
            std::make_shared<cc::RandomNumberGenerator>(rngSeed_));

        rngSeed_++;
    }

    checkpointSaver_ = std::make_unique<cc::CheckpointSaver>(
            experimentConfiguration->getCheckpointSaverParameters());
}

void cc::CgpTrainer::run() {
    if (islands_.size() == 0) {
        throw std::runtime_error("There are no islands. Check the configuration.");
    }

    long numEpochsRun = 0;
    int numIterationsPerEpoch = islands_[0].getNumIterationsPerEpoch();
    bestIslandIndex_ = -1;
    bestFitnessScore_ = std::numeric_limits<double>::max();

    std::chrono::high_resolution_clock clk;

    std::chrono::high_resolution_clock::time_point startTime = clk.now();

    omp_set_num_threads(maxNumThreads_);

    if (consoleFrequency_ > 0) {
        int result = std::system("clear");
        std::cout << "Number of epochs | best fitness score | Elapsed time (us)" << std::endl;
    }

    while ((numEpochsRun < terminationNumEpochs_)
            && (bestFitnessScore_ > terminationFitness_)) {

        #pragma omp parallel for default(none) shared(islands_)
        for (size_t i = 0; i < islands_.size(); i++) {
            islands_[i].runEpoch();
        }

        for (size_t i = 0; i < islands_.size(); i++) {
            double islandFitness = islands_[i].getBestFitness();
            if (islandFitness < bestFitnessScore_) {
                bestFitnessScore_ = islandFitness;
                bestIslandIndex_ = i;
            }
        }

        const cc::Genotype& bestGenotype = islands_[bestIslandIndex_].getBestGenotype();
        for (size_t i = 0; i < islands_.size(); i++) {
            if (i != bestIslandIndex_) {
                islands_[i].setResidentGenotypes(bestGenotype, bestFitnessScore_);
            }
        }

        numEpochsRun++;

        if ((checkpointFrequency_ > 0)
            && ((numEpochsRun % checkpointFrequency_) == 0)) {
            std::chrono::high_resolution_clock::time_point now = clk.now();

            cc::CheckpointLogInformation_t checkpointInformation;
            checkpointInformation.cumulativeNumberOfEpochs = numEpochsRun;
            checkpointInformation.cumulativeElapsedTimeUs = 
                std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
            checkpointInformation.bestFitness = bestFitnessScore_;
            checkpointInformation.isFinal = false;

            checkpointSaver_->saveCheckpoint(checkpointInformation, bestGenotype);
        }

        if ((consoleFrequency_ > 0)
            && ((numEpochsRun % consoleFrequency_) == 0)) {
            std::chrono::high_resolution_clock::time_point now = clk.now();
            double elapsedUs = 
                std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();

            std::cout << 
                numEpochsRun
                << " | "
                << std::to_string(bestFitnessScore_)
                << " | " 
                << std::to_string(elapsedUs)
                << std::endl;
        }
    }

    const cc::Genotype& endingGenotype = islands_[bestIslandIndex_].getBestGenotype();

    cc::CheckpointLogInformation_t finalCpInformation;
    finalCpInformation.cumulativeNumberOfEpochs = numEpochsRun;
    finalCpInformation.cumulativeElapsedTimeUs = 
        std::chrono::duration_cast<std::chrono::microseconds>(clk.now() - startTime).count();
    finalCpInformation.bestFitness = bestFitnessScore_;
    finalCpInformation.isFinal = true;

    checkpointSaver_->saveCheckpoint(finalCpInformation, endingGenotype);
}

const cc::Genotype& cc::CgpTrainer::getBestGenotype() {
    return islands_[bestIslandIndex_].getBestGenotype();
}

double cc::CgpTrainer::getBestGenotypeFitness() {
    return bestFitnessScore_;
}

void cc::CgpTrainer::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& parameters) {
    maxNumThreads_ = std::stoi(parameters.at("maxNumThreads"));
    numIslands_ = std::stoi(parameters.at("numIslands"));
    rngSeed_ = std::stoi(parameters.at("rngSeed"));
    terminationFitness_ = std::stod(parameters.at("terminationFitness"));
    terminationNumEpochs_ = std::stod(parameters.at("terminationNumEpochs"));
    checkpointFrequency_ = std::stoi(parameters.at("checkpointFrequency"));
    consoleFrequency_ = std::stoi(parameters.at("consoleFrequency"));
}
