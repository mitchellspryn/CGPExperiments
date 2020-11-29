#include "../include/GenePool.hpp"

#include <utility>

namespace cc = cgpExperiments::core;

cc::GenePool::GenePool(
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration,
        std::shared_ptr<cc::GeneFactory> geneFactory,
        std::unique_ptr<cc::RandomNumberGenerator> randomNumberGenerator) {
    experimentConfiguration_ = experimentConfiguration;
    geneFactory_ = geneFactory;
    randomNumberGenerator_ = std::move(randomNumberGenerator);

    fillParametersFromMap(experimentConfiguration->getGenePoolParameters());

    initializePools();
}

std::unique_ptr<cc::Gene> cc::GenePool::getFromPool(const std::string& geneName) {
    //std::lock_guard<std::mutex> guard(mutexes_.at(geneName));
    //std::stack<std::unique_ptr<Gene>>& currentPool = pools_.at(geneName);
    //if (currentPool.empty()) {
    //    fillPool(geneName, currentPool);
    //}

    //std::unique_ptr<cc::Gene> output = std::move(currentPool.top());
    //currentPool.pop();

    //return output;
    
    // OK, this gene pool thing turned out to be a terrible idea. 
    // Valgrind shows that a ton of time is spent in the hashsets.
    // Also, the threads frequently block on each other waiting for locks to be released.
    // It can probably be optimized further, but for now just creating a new one seems fast enough.
    // Premature optimization...
    return std::move(geneFactory_->createGene(geneName));
}

std::unique_ptr<cc::Gene> cc::GenePool::getRandomGeneFromPool() {
    int randomNameIndex = randomNumberGenerator_->getRandomInt(0, geneNames_.size() - 1);
    return getFromPool(geneNames_[randomNameIndex]);
}

void cc::GenePool::returnToPool(std::unique_ptr<cc::Gene> gene) {
    //std::string geneName = gene->getGeneName();
    //std::lock_guard<std::mutex> guard(mutexes_.at(geneName));
    //pools_.at(geneName).push(std::move(gene));
}

void cc::GenePool::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& params) {
    initialPoolSize_ = std::stoi(params.at("initialPoolSize"));
}

void cc::GenePool::initializePools() {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> allGeneParameters = experimentConfiguration_->getAllGeneParameters();

    for (auto it = allGeneParameters.begin(); it != allGeneParameters.end(); ++it) {
        const std::string& geneName = it->first;
        
        // Use [] to default-construct mutex in place
        std::mutex& dummy = mutexes_[geneName];
        std::stack<std::unique_ptr<cc::Gene>>& pool = pools_[geneName];

        fillPool(geneName, pool);
        geneNames_.emplace_back(geneName);
    }
}

void cc::GenePool::fillPool(
        const std::string& geneName,
        std::stack<std::unique_ptr<cc::Gene>>& poolToExpand) {
    while (poolToExpand.size() < initialPoolSize_) {
        poolToExpand.push(std::move(geneFactory_->createGene(geneName)));
    }
}

