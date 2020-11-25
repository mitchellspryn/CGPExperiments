#include "../include/GeneFactory.hpp"

#include <iterator>

#include "RandomNumberGenerator.hpp"

namespace cc = cgpExperiments::core;

std::unique_ptr<cc::Gene> cc::GeneFactory::createRandomGene(
        const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& geneParameters) {
    if (geneNames_.size() == 0) {
        extractGeneNames(geneParameters);
    }

    int randomGeneIndex = cc::randomNumberGenerator::getRandomInt(0, geneNames_.size() - 1);


    std::unique_ptr<Gene> gene = createGene(geneNames_[randomGeneIndex]);
    gene->initializeFromParameters(geneParameters[geneNames_[randomGeneIndex]]);

    return gene;
}

void cc::GeneFactory::extractGeneNames(
        const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& geneParameters) {
    geneNames_.reserve(geneParameters.size());

    for (auto it = geneParameters.begin(); it != geneParameters.end(); ++it) {
       geneNames_.emplace_back(it->first);
    }
}
