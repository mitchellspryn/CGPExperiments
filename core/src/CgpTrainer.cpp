#include "../include/CgpTrainer.hpp"

namespace cc = cgpExperiments::core;

cc::CgpTrainer::CgpTrainer(
        std::shared_ptr<cc::FitnessFunctionFactory> fitnessFunctionFactory,
        std::shared_ptr<cc::GeneFactory> geneFactory,
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration) {
    experimentConfiguration_ = experimentConfiguration;
    fitnessFunctionFactory_ = fitnessFunctionFactory;

    fillParametersFromMap(fitnessFunctionFactory_->getTrainerParameters());

    genePool_ = std::make_shared<cc::GenePool>(
        experimentConfiguration_,
        geneFactory_);

    std::vector<std::unordered_map<std::string, std::string>> inputDataChunkParameters =
        experimentConfiguration_->getInputDataChunkProviderParameters();
    inputDataChunkProviderParameters_.reserve(inputDataChunkParameters.size());

    for (size_t i = 0; i < inputDataChunkParameters.size(); i++) {
        inputDataChunkProviders_.emplace_back(
            std::make_shared<cc::DataChunkProvider>(
                inputDataChunkProviderParameters_[i]));
    }

    labelDataChunkProvider_ = 
        std::make_shared<cc::DataChunkProvider>(
            experimentConfiguration_->getLabelDataChunkProviderParameters());

    for (size_t i = 0; i < numIslands_; i++) {
        islands_.emplace_back(
            fitnessFunctionFactory_,
            geneFactory_,
            inputdDataChunkProviders_,
            labelDataChunkProvider_,
            experimentConfiguration);
    }
}

cc::CgpTrainer::run() {
    long numIterationsRun = 0;
    int numIterationsPerEpoch = islands_[i]->getNumIterationsPerEpoch();
    bestIslandIndex_ = -1;
    bestFitnessScore_ = std::numeric_limits<double>::max();

    while ((numIterationsRun < terminationNumIterations_)
            && (bestFitnessScore > terminationFitness_)) {
        // TODO: parallelize this loop with openMP
        for (size_t i = 0; i < islands_.size(); i++) {
            islands_[i]->runEpoch();
        }

        numIterationsRun += numIterationsPerEpoch;

        for (size_t i = 0; i < islands_.size(); i++) {
            double islandFitness = islands_[i]->getBestFitness();
            if (islandFitness < bestFitnessScore_) {
                bestFitnessScore = islandFitness;
                bestIslandIndex_ = i;
            }
        }

        const cc::Genotype& bestGenotype = islands[bestIslandIndex_]->getBestGenotype();
        for (size_t i = 0; i < islands_.size(); i++) {
            if (i != bestIslandIndex_) {
                islands_[i]->setResidentGenotypes(bestGenotype, bestFitnessScore);
            }
        }
    }
}

const cc::Genotype& cc::CgpTrainer::getBestGenotype() {
    return islands_[bestIslandIndex_]->getBestGenotype();
}

double cc::CgpTrainer::getBestGenotypeFitness() {
    return bestFitness_;
}

void cc::CgpTrainer::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& parameters) {
    maxNumThreads_ = std::stoi(parameters["maxNumThreads"]);
    numIslands_ = std::stoi(parameters["numIslands"]);
    terminationFitness_ = std::stod(parameters["terminationFitness"]);
    terminationNumIterations_ = std::stod(paramters["terminationNumIterations"]);
}
