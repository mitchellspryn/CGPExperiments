#include "../include/Island.hpp"

namespace cc = cgpExperiments::core;

cc::Island::Island(
        std::shared_ptr<FitnessFunctionFactory> fitnessFunctionFactory,
        std::shared_ptr<GenePool> genePool,
        const std::vector<std::shared_ptr<DataChunkProvider>>& inputDataChunkProviders,
        const std::shared_ptr<DataChunkProvider>& labelDataChunkProvider,
        std::shared_ptr<ExperimentConfiguration> experimentConfiguration) {
    experimentConfiguration_ = experimentConfiguration;
    genePool_ = genePool;
    inputDataChunkProviders_ = inputDataChunkProviders;
    labelDataChunkProvider_ = labelDataChunkProvider;
    fitnessFunction_ = fitnessFunctionFactory->create(
            experimentConfiguration_->getFitnessFunctionParameters());

    fillParametersFromMap(experimentConfiguration_->getIslandParameters());

    for (int i = 0; i < island.numGenotypes_; i++) {
        residents_.emplace_back(experimentConfiguration_, genePool_);
        residents_[i]->initializeRandomly();
    }

    for (size_t i = 0; i < inputDataChunkProviders_.size(); i++) {
        inputDataChunkBuffers_[i].reset(new cc::DataChunk(
            inputDataChunkProviders_[i]->getSampleWidth(),
            inputDataChunkProviders_[i]->getSampleHeight(),
            numEvaluationSamples_));
    }

    labelDataChunkBuffer_->reset(new cc::DataChunk(
            labelDataChunkProvider_->getSampleWidth(),
            labelDataChunkProvider_->getSampleHeight(),
            numEvaluationSamples_));
}

int cc::Island::getNumIterationsPerEpoch() {
    return numIterationsPerEpoch_;
}

void cc::Island::runEpoch() {
    // TODO: technically, this should be inside the loops. 
    // But, if the genes do not modify the input buffers (as they should not),
    // then we can do it once on the outside. 
    //
    // We should figure out a way to make this enforcable by the compiler.
    int chunkStartIndex = evalStartIndex_;
    if (chunkStartIndex < 0) {
        chunkStartIndex = 
            cc::RandomNumberGenerator::getRandomInt(
                0, 
                inputDataChunkProviders_[0]->getNumSamplesInDataset() - 1);
    }

    for (size_t j = 0; j < inputDataChunkBuffers_[i].size(); j++) {
        inputDataChunkProviders_[j]->getRandomChunk(*inputDataChunkBuffers_[i], chunkStartIndex);
    }

    labelDataChunkProvider_->getRandomChunk(*labelDataChunkBuffer_, chunkStartIndex);

    // Find the initial parent only if it hasn't been set.
    if (bestFitnessIndex_ < 0) {
        for (int i = 0; i < numGenotypes_; i++) {
            const cc::DataChunk& predictions = residents_[i]->evaluate(inputDataChunkBuffers_);

            double fitness = fitnessFunction_->evaluate(
                predictions,
                *labelDataChunkProvider_,
                *residents_[i]);

            if (fitness <= bestFitness_) {
                bestFitnessIndex_ = i; 
                bestFitness_ = fitness;
            }
        }
    }

    for (int iter = 0; iter < numIterationsPerEpoch_; iter++) {
        for (int i = 0; i < numGenotypes_; i++) {
            if (i != bestFitnessIndex_) {
                residents_[i]->setGenes(
                    residents_[bestFitnessIndex_]->getGenes(),
                    residents_[bestFitnessIndex_]->getActiveGenes());

                residents_[i]->mutate();

                double fitness = fitnessFunction_->evaluate(
                    predictions,
                    *labelDataChunkProvider_,
                    *residents_[i]);

                if (fitness <= bestFitness_) {
                    bestFitnessIndex_ = i; 
                    bestFitness_ = fitness;
                }
            }
        }
    }
}

double cc::Island::getBestFitness() {
    return bestFitness_;
}

const cc::Genotype& cc::Island::getBestGenotype() {
    return residents_[bestFitnessIndex_];
}

void cc::Island::setResidentGenotypes(const cc::Genotype& genotype, double fitness) {
    for (int i = 0; i < numGenotypes_; i++) {
        residents[i]->setGenes(
                genotype.getGenes(),
                genotype.getActiveGenes());
    }

    // All genes are the same, so take the first one to be the parent.
    bestFitnessIndex_ = 0;
    bestFitness_ = fitness;
}


void cc::Island::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& params) {
    numGenotypes_ = std::stoi(params.at("numGenotypes"));
    numIterationsPerEpoch_ = std::stoi(params.at("numIterationsPerEpoch"));
    numEvaluationSamples_ = std::stoi(params.at("numEvaluationSamples"));
    evalStartIndex_ = std::stoi(params.at("evalStartIndex"));
}
