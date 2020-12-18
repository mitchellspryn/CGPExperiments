#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../core/include/Gene.hpp"
#include "../../core/include/GeneFactory.hpp"

namespace cgpExperiments {
namespace image {

class ImageGeneFactory : public cgpExperiments::core::GeneFactory {
    public:
        virtual ~ImageGeneFactory() override {};
        virtual std::unique_ptr<cgpExperiments::core::Gene> createGene(
            int typeId) override;
        virtual int getTypeId(
            const std::string& geneName) override;
};

}
}
