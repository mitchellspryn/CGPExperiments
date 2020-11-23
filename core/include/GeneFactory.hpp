#pragma once

#include <string>
#include <unordered_map>

#include "Gene.hpp"

namespace cgpExperiments {
namespace core {

class GeneFactory {
    public:
        GeneFactory(std::unordered_map<std::string, std::unordered_map<std::string, std::string>> geneInitializationParameters);
        void CreateRandomGene(std::vector<Gene>& genes, int index);

    protected:
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> geneInitializationParmaeters_;
        void CreateGene(Gene& gene, std::string geneName);
};

}
}
