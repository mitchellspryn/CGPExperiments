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

    nlohmann::json j;
    std::ifstream stream(inputJsonFilePath);
    if (!stream.good()) {
        throw std::runtime_error(
            "Could not open config file at '"
            + inputJsonFilePath
            + "'.");
    }

    stream >> j;

    // For some reason, template deduction in nlohmann::json fails with class member variables
    // Create dummy variables to allow type inference to work.
    std::string tmp1 = j["geneset"];
    geneset_ = tmp1;

    auto fillFromKeyValue = 
        [](nlohmann::json jobj, std::unordered_map<std::string, std::string>& map) {
        for (nlohmann::json::iterator it = jobj.begin(); it != jobj.end(); ++it) {
            std::string key = it.key();

            // Calling .dump() on a string will escape quotes
            if (it.value().type() == nlohmann::json::value_t::string) {
                map[key] = it.value().get<std::string>();
            } else {
                map[key] = it.value().dump();

            }
        }
    };

    fillFromKeyValue(j["trainerParameters"], trainerParameters_);
    fillFromKeyValue(j["islandParameters"], islandParameters_);
    fillFromKeyValue(j["genotypeParameters"], genotypeParameters_);
    fillFromKeyValue(j["fitnessFunctionParameters"], fitnessFunctionParameters_);
    fillFromKeyValue(j["genePoolParameters"], genePoolParameters_);
    fillFromKeyValue(j["checkpointSaverParameters"], checkpointSaverParameters_);

    nlohmann::json geneParametersObj = j["geneParameters"];
    for (nlohmann::json::iterator it = geneParametersObj.begin(); it != geneParametersObj.end(); ++it) {
        std::string geneName = it.key();
        fillFromKeyValue(geneParametersObj[geneName], geneParameters_[geneName]);
    }

    nlohmann::json arr = j["inputDataChunkParameters"];
    for (nlohmann::json::iterator it = arr.begin(); it != arr.end(); ++it) {
        std::unordered_map<std::string, std::string> tmp;
        fillFromKeyValue(*it, tmp);
        inputDataChunkProviderParameters_.emplace_back(tmp);
    }

    fillFromKeyValue(j["labelDataChunkParameters"], labelDataChunkProviderParameters_);
}
