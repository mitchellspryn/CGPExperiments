#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../../core/include/FitnessFunctionFactory.hpp"

namespace cgpExperiments {
namespace image {

class ImageFitnessFunctionFactory : public cgpExperiments::core::FitnessFunctionFactory {
    public:
        virtual ~ImageFitnessFunctionFactory() override {};
        virtual std::unique_ptr<cgpExperiments::core::FitnessFunction> create(
            const std::unordered_map<std::string, std::string>& fitnessFunctionParameters) override;
};

}
}
