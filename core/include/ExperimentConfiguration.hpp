#pragma once

#include <string>
#include <unordered_map>

namespace cgpExperiments {
namespace core {

class ExperimentConfiguration {
    public:
        ExperimentConfiguration(const std::string& inputJsonFilePath); 

        inline const std::unordered_map<std::string, std::string>& getTrainerParameters() const {
            return trainerParameters_;
        }

        inline const std::unordered_map<std::string, std::string>& getIslandParameters() const {
            return islandParameters_;
        }

        inline const std::unordered_map<std::string, std::string>& getGenotypeParameters() const {
            return genotypeParameters,
        }

        inline const std::unordered_map<std::string, std::string>& getFitnessFunctionParameters() const {
            return fitnessFunctionParameters_;
        }

        inline const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& getAllGeneParameters() const {
            return geneParameters_;
        }

        inline const std::unordered_map<std::string, std::string>& getGeneParameters(const std::string& geneName) const {
            return geneParameters_[geneName];
        }

    private:
        std::unordered_map<std::string, std::string> trainerParameters_;
        std::unordered_map<std::string, std::string> islandParameters_;
        std::unordered_map<std::string, std::string> genotypeParameters_;
        std::unordered_map<std::string, std::string> fitnessFunctionParameters_;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string> geneParameters_;

};

}
}
