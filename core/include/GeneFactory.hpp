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
        virtual ~GeneFactory() {};
        virtual std::unique_ptr<Gene> createGene(
            int typeId) = 0;
        virtual int getTypeId(
            const std::string& geneName) = 0;
};

}
}
