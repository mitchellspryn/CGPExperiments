#pragma once

#include <memory>
#include <mutex>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "ExperimentConfiguration.hpp"
#include "Gene.hpp"
#include "GeneFactory.hpp"
#include "RandomNumberGenerator.hpp"

namespace cgpExperiments {
namespace core {

class GenePool {
    public:
        GenePool(
            std::shared_ptr<ExperimentConfiguration> experimentConfiguration,
            std::shared_ptr<GeneFactory> geneFactory,
            std::unique_ptr<RandomNumberGenerator> randomNumberGenerator);

        std::unique_ptr<Gene> getFromPool(int geneTypeId);
        std::unique_ptr<Gene> getRandomGeneFromPool();
        void returnToPool(std::unique_ptr<Gene> gene);

    private:
        std::vector<std::stack<std::unique_ptr<Gene>>> pools_;
        std::vector<std::mutex> mutexes_;

        //std::unordered_map<std::string, std::stack<std::unique_ptr<Gene>>> pools_;
        //std::unordered_map<std::string, std::mutex> mutexes_;

        std::shared_ptr<ExperimentConfiguration> experimentConfiguration_;
        std::shared_ptr<GeneFactory> geneFactory_;
        std::vector<int> activeGenes_;
        std::unique_ptr<RandomNumberGenerator> randomNumberGenerator_;

        int initialPoolSize_;

        void fillParametersFromMap(
            const std::unordered_map<std::string, std::string>& params);
        void initializePools();
        void fillPool(
            int geneTypeId,
            std::stack<std::unique_ptr<Gene>>& poolToExpand);
};

}
}
