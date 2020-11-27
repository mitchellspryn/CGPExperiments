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

    nlohmann::json::json j;
    std::ifstream stream(inputJsonFilePath);
    if (!stream.good()) {
        throw std::runtime_error(
            "Could not open config file at '"
            + inputJsonFilePath
            + "'.");
    }

    stream >> j;

    geneSet_ = j["geneset"];

    auto fillFromKeyValue = 
        [](json::json jobj, std::unordered_map<std::string, std::string>& map) {
        for (json::iterator it = jobj.begin(); it != jobj.end(); ++it) {
            map[it.key().get<std::string>()] = it.value().get<std::string>();
        }
    };

    fillFromKeyValue(j["trainerParameters"], trainerParameters_);
    fillFromKeyValue(j["islandParameters"], islandParameters_);
    fillFromKeyValue(j["genotypeParameters"], genotypeParameters_);
    fillFromKeyValue(j["fitnessFunctionParameters"], fitnessFunctionParameters_);
    fillFromKeyValue(j["genePoolParameters"], genePoolParameters_);
    fillFromKeyValue(j["checkpointSaverParameters"], checkpointSaverParameters_);

    json::object geneParametersObj = j["geneParameters"];
    for (json::iterator it = geneParametersObj.begin(); it != geneParametersObj.end(); ++it) {
        std::string geneName = it.key().get<std::string>();
        fillFromKeyValue(geneParametersObj[geneName], geneParameters_[geneName]);
    }

    json::array arr = j["inputDataChunkParameters"];
    for (json::iterator it = arr.begin(); it != arr.end(); ++it) {
        std::map<std::string, std::string> tmp;
        fillFromKeyValue(json::object(*it), tmp);
        inputDataChunkProviderParameters_.emplace_back(tmp);
    }

    fillFromKeyValue(j["labelDataChunkParameters"], labelDataChunkProviderParameters_);
}
