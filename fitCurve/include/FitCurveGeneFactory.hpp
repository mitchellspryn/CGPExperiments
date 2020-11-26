#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../core/include/Gene.hpp"
#include "../../core/include/GeneFactory.hpp"

namespace cgpExperiments {
namespace fitCurve {

class FitCurveGeneFactory : public cgpExperiments::core::GeneFactory {
    public:
        virtual ~FitCurveGeneFactory() override {};
        virtual std::unique_ptr<cgpExperiments::core::Gene> createGene(
            const std::string& geneName) override;
};

}
}
