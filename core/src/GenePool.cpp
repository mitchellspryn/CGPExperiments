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

std::unique_ptr<cc::Gene> cc::GenePool::getFromPool(int geneTypeId) {
    //std::lock_guard<std::mutex> guard(mutexes_.at(geneTypeId));
    //std::stack<std::unique_ptr<Gene>>& currentPool = pools_.at(geneTypeId);
    //if (currentPool.empty()) {
    //    fillPool(geneTypeId, currentPool);
    //}

    //std::unique_ptr<cc::Gene> output = std::move(currentPool.top());
    //currentPool.pop();

    //return output;
    
    // OK, this gene pool thing turned out to be a terrible idea. 
    // The threads frequently block on each other waiting for locks to be released.
    // Also, manipulating the stack seems to take more time than doing the malloc.
    //
    // Lots of time is spent in malloc / free, but to make it faster, 
    //     we'd have to give up the class hierarchy. Which might be worse for flexability.
    return std::move(geneFactory_->createGene(geneTypeId));
}

std::unique_ptr<cc::Gene> cc::GenePool::getRandomGeneFromPool() {
    int randomNameIndex = randomNumberGenerator_->getRandomInt(0, activeGenes_.size() - 1);
    return getFromPool(activeGenes_[randomNameIndex]);
}

void cc::GenePool::returnToPool(std::unique_ptr<cc::Gene> gene) {
    //int geneTypeId = gene->getTypeId();
    //std::lock_guard<std::mutex> guard(mutexes_.at(geneTypeId));
    //pools_.at(geneTypeId).push(std::move(gene));
}

void cc::GenePool::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& params) {
    initialPoolSize_ = std::stoi(params.at("initialPoolSize"));
}

void cc::GenePool::initializePools() {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> allGeneParameters = experimentConfiguration_->getAllGeneParameters();

    int maxTypeId = std::numeric_limits<int>::min();
    for (auto it = allGeneParameters.begin(); it != allGeneParameters.end(); ++it) {
        const std::string& geneName = it->first;
        int geneTypeId = geneFactory_->getTypeId(geneName);
        maxTypeId = std::max(geneTypeId, maxTypeId);
    }

    mutexes_ = std::vector<std::mutex>(maxTypeId+1);
    pools_ = std::vector<std::stack<std::unique_ptr<cc::Gene>>>(maxTypeId+1);

    for (auto it = allGeneParameters.begin(); it != allGeneParameters.end(); ++it) {
        const std::string& geneName = it->first;
        int geneTypeId = geneFactory_->getTypeId(geneName);
        
        fillPool(geneTypeId, pools_[geneTypeId]);
        activeGenes_.emplace_back(geneTypeId);
    }
}

void cc::GenePool::fillPool(
        int geneTypeId,
        std::stack<std::unique_ptr<cc::Gene>>& poolToExpand) {
    while (poolToExpand.size() < initialPoolSize_) {
        poolToExpand.push(std::move(geneFactory_->createGene(geneTypeId)));
    }
}

