#include "../include/Island.hpp"

namespace cc = cgpExperiments::core;

cc::Island::Island(
        std::shared_ptr<FitnessFunctionFactory> fitnessFunctionFactory,
        std::shared_ptr<GeneFactory> geneFactory,
        const std::vector<std::shared_ptr<DataChunkProvider>>& inputDataChunkProviders,
        const std::shared_ptr<DataChunkProvider>& labelDataChunkProvider,
        std::shared_ptr<RandomNumberGenerator> rng) {
    experimentConfiguration_ = experimentConfiguration;
    rng_ = rng;
    geneFactory_ = geneFactory;
    inputDataChunkProviders_ = inputDataChunkProviders;
    labelDataChunkProvider_ = labelDataChunkProvider;
    fitnessFunction_ = fitnessFunctionFactory->create(
            experimentConfiguration_->getFitnessFunctionParameters());

    fillParametersFromMap(experimentConfiguration_->getIslandParameters());

    for (int i = 0; i < island.numGenotypes_; i++) {
        residents_.emplace_back(experimentConfiguration_, rng, geneFactory_);
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

void cc::Island::runEpoch() {
    // TODO: technically, this should be inside the loops. 
    // But, if the genes do not modify the input buffers (as they should not),
    // then we can do it once on the outside. 
    //
    // We should figure out a way to make this enforcable by the compiler.
    int chunkStartIndex = evalStartIndex_;
    if (chunkStartIndex < 0) {
        chunkStartIndex = 
            rng_->getRandomInt(0, inputDataChunkProviders_[0]->getNumSamplesInDataset() - 1);
    }

    for (size_t j = 0; j < inputDataChunkBuffers_[i].size(); j++) {
        inputDataChunkProviders_[j]->getRandomChunk(*inputDataChunkBuffers_[i], chunkStartIndex);
    }

    labelDataChunkProvider_->getRandomChunk(*labelDataChunkBuffer_, chunkStartIndex);

    // Find the initial parent
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

    for (int iter = 1; iter < numIterationsPerEpoch_; initializeRandomly++) {
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

const cc::Genotype& cc::Island::getBestGenotype() {
    return residents_[bestFitnessIndex_];
}

double cc::Island::getBestFitness() {
    return bestFitness_;
}

void cc::Island::setResidentGenotypes(const cc::Genotype& genotype, double fitness) {
    for (int i = 0; i < numGenotypes_; i++) {
        residents[i]->setGenes(
                genotype.getGenes(),
                genotype.getActiveGenes());
    }
}


void cc::Island::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& params) {
    numGenotypes_ = std::stoi(params["numGenotypes"]);
    numIterationsPerEpoch_ = std::stoi(params["numIterationsPerEpoch"]);
    numEvaluationSamples_ = std::stoi(params["numEvaluationSamples"]);
    evalStartIndex_ = std::stoi(parames["evalStartIndex"]);
}
