#include "../include/ExperimentConfiguration.hpp"

#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace cc = cgpExperiments::core;

cc::ExperimentConfiguration::ExperimentConfiguration(const std::string& inputJsonFilePath) {
    trainerParameters_.clear();
    islandParameters_.clear();
    genotypeParameters_.clear();
    geneParameters_.clear();

    nlohmann::json::json json;
    std::ifstream stream(inputJsonFilePath);
    if (!stream.good()) {
        throw std::runtime_error(
            "Could not open config file at '"
            + inputJsonFilePath
            + "'.");
    }

    stream >> json;

    auto fillFromKeyValue = 
        [&](json::object jobj, std::unordered_map<std::string, std::string>& map) {
        for (json::iterator it = jobj.begin(); it != jobj.end(); ++it) {
            map[it.key().get<std::string>()] = it.value().get<std::string>();
        }
    };

    fillFromKeyValue(json["trainerParameters"], trainerParameters_);
    fillFromKeyValue(json["islandParameters"], islandParameters_);
    fillFromKeyValue(json["genotypeParameters"], genotypeParameters_);
    fillFromKeyValue(json["fitnessFunctionParameters"], fitnessFunctionParameters_);

    json::object geneParametersObj = json["geneParameters"];
    for (json::iterator it = geneParametersObj.begin(); it != geneParametersObj.end(); ++it) {
        std::string geneName = it.key().get<std::string>();
        fillFromKeyValue(geneParametersObj[geneName], geneParameters_[geneName]);
    }
}
