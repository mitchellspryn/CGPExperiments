#include "../include/GenePool.hpp"

#include <utility>

#include "../include/RandomNumberGenerator.hpp"

namespace cc = cgpExperiments::core;

cc::GenePool::GenePool(
        std::shared_ptr<ExperimentConfiguration> experimentConfiguration,
        std::shared_ptr<GeneFactory> geneFactory) {
    experimentConfiguration_ = experimentConfiguration;
    geneFactory_ = geneFactory;

    fillParametersFromMap(experimentConfiguration->getGenePoolParameters());

    initializeCaches();
}

std::unique_ptr<cc::Gene> cc::GenePool::getFromPool(const std::string& geneName) {
    std::lock_guard<std::mutex> guard(mutexes_.at(geneName));
    std::stack<std::unique_ptr<Gene>>& currentPool = pools_.at(geneName);
    if (currentStack.empty()) {
        fillPool(geneName, currentPool);
    }

    std::unique_ptr<cc::Gene> output = std::move(currentPool.top());
    currentPool.pop();

    return output;
}

std::unique_ptr<cc::Gene> cc::GenePool::getRandomGeneFromPool() {
    int randomNameIndex = cc::randomNumberGenerator::getRandomInt(0, geneNames_.size() - 1);
    return getFromPool(geneNames_[randomNameIndex]);
}

void cc::GenePool::returnToPool(std::unique_ptr<cc::Gene> gene) {
    std::lock_guard<std::mutex> guard(mutexes_.at(geneName));
    pools_.at(geneName).push(std::move(gene));
}

void cc::GenePool::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& params) {
    initialPoolSize_ = std::stoi(params["initialPoolSize"]);
    poolSizeMultiplier_ = std::stod(params["poolSizeMultiplier"]);
}

void cc::GenePool::initializePools() {
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> allGeneParameters = experimentConfig->getAllGeneParameters();

    for (auto it = allGeneParameters.begin(); it != allGeneParameters.end(); ++it) {
        std::string& geneName = it->first;
        
        // Use [] to default-construct mutex in place
        std::mutex dummy = mutexes_[geneName];
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

