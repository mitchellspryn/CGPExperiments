#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Gene.hpp"
#include "RandomNumberGenerator.hpp"

namespace cgpExperiments {
namespace core {

class GeneFactory {
    public:
        GeneFactory(
                const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& defaultGeneInitializationParameters,
                std::shared_ptr<RandomNumberGenerator> rng) {
            rng_ = rng; 
            defaultGeneInitializationParameters_ = defaultGeneInitializationParameters;
        }

        std::unique_ptr<Gene> createRandomGene();
        std::unique_ptr<Gene> createGene(const std::string& geneTypeName);
        virtual std::unique_ptr<Gene> createGene(
                const std::string& geneTypeName,
                const std::unordered_map<std::string, std::string>& geneParameters) = 0;

    protected:
        std::shared_ptr<RandomNumberGenerator> rng_;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> defaultGeneInitializationParameters_;
};

}
}
