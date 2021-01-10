#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Gene.hpp"

namespace cgpExperiments {
namespace core {

// A class that can create a new gene.
class GeneFactory {
    public:
        virtual ~GeneFactory() {};

        // Given a gene type id, create a new gene.
        // This should be overriden in custom geneset implementations.
        // This will be called many times during the training process.
        virtual std::unique_ptr<Gene> createGene(
            int typeId) = 0;

        // Get a gene type id for a gene name.
        // This will be called once on startup.
        // The gene IDs will be used afterwards for performance.
        virtual int getTypeId(
            const std::string& geneName) = 0;
};

}
}
