#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Gene.hpp"

namespace cgpExperiments {
namespace core {

class GeneFactory {
    public:
        virtual std::unique_ptr<Gene> createGene(
            const std::string& geneTypeName) = 0;
};

}
}
