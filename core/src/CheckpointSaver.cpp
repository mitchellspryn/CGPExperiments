#include "../include/CheckpointSaver.hpp"

#include <cstdlib>
#include <fstream>
#include <filesystem>
#include <strings.h>
#include <vector>

#include <nlohmann/json.hpp>

namespace cc = cgpExperiments::core;

cc::CheckpointSaver::CheckpointSaver(
        const std::unordered_map<std::string, std::string>& checkpointSaverParameters) {
    parseParameters(checkpointSaverParameters);

    // If the directory exists, does nothing.
    std::filesystem::create_directories(outputRootDirectory_);

    // Write log header.
    logFileName_ = outputRootDirectory_ + "/log.txt";
    std::ofstream logFile(logFileName_);
    if (!logFile.good()) {
        throw std::runtime_error(
            "Attempted to open log file at '"
            + logFileName_
            + "', but it could not be opened.");
    }

    logFile << "cumulativeNumberOfEpochs,bestFitness,cumulativeElapsedTimeUs\n";
}

void cc::CheckpointSaver::saveCheckpoint(
        const cc::CheckpointLogInformation_t& checkpointLogInformation,
        const cc::Genotype& bestGenotype,
        const cc::DataChunk& predictions,
        const std::vector<std::string>& inputDataSetNames) {
    appendLogInformation(checkpointLogInformation);

    std::string iterationDirectory = outputRootDirectory_ + "/";
    if (checkpointLogInformation.isFinal) {
        iterationDirectory += "final";
    } else {
        iterationDirectory += 
            std::to_string(checkpointLogInformation.cumulativeNumberOfEpochs);
    }

    std::filesystem::create_directory(iterationDirectory);

    saveGenotypeToJson(iterationDirectory, bestGenotype);

    if (generateCode_) {
        saveGenotypeToCode(iterationDirectory, bestGenotype);
    }

    if (generateImage_) {
        saveGenotypeImage(iterationDirectory, bestGenotype, inputDataSetNames);
    }

    if (generatePredictions_) {
        savePredictions(iterationDirectory, predictions);
    }
}

void cc::CheckpointSaver::appendLogInformation(
        const cc::CheckpointLogInformation_t& checkpointLogInformation) {
    std::ofstream logFile(logFileName_, std::ios_base::app);

    if (!logFile.good()) {
        throw std::runtime_error(
            "Could not open '"
            + logFileName_
            + "' in append mode.");
    }

    logFile 
        << std::to_string(checkpointLogInformation.cumulativeNumberOfEpochs)
        << ","
        << std::to_string(checkpointLogInformation.bestFitness)
        << ","
        << std::to_string(checkpointLogInformation.cumulativeElapsedTimeUs)
        << "\n";
}

void cc::CheckpointSaver::saveGenotypeToJson(
        const std::string& outputDirectory,
        const cc::Genotype& genotype) {
    std::string outputFileName = outputDirectory + "/genotype.json";
    std::vector<std::unordered_map<std::string, std::string>> serialized = genotype.serialize();

    nlohmann::json json(serialized);
    std::string jsonStr = json.dump(4);

    std::ofstream stream(outputFileName);
    stream << jsonStr;
}

void cc::CheckpointSaver::saveGenotypeToCode(
        const std::string& outputDirectory,
        const cc::Genotype& genotype) {

    std::string outputFileName = outputDirectory + "/genotype.cpp";

    // TODO: make language configurable when more than one type is supported.
    cc::CodeGenerationContext_t context;
    context.generationLanguage = cc::GenerationLanguage::CPP;

    std::string generatedCode = genotype.generateCode(context);

    std::ofstream stream(outputFileName);
    stream << generatedCode;
}

void cc::CheckpointSaver::saveGenotypeImage(
        const std::string& outputDirectory, 
        const cc::Genotype& genotype,
        const std::vector<std::string>& inputDataSetNames) {
    // TODO: should we allow active visibility to be togglable?
    std::string activeDotFilePath = outputDirectory + "/dotActiveGenes.gv";
    std::string allDotFilePath = outputDirectory + "/dotAllGenes.gv";
    std::string activeImagePath = outputDirectory + "/picActiveGenes.png";
    std::string allImagePath = outputDirectory + "/picAllGenes.png";

    std::string activeDotFileText = genotype.generateDotFile(false, inputDataSetNames);
    std::string allDotFileText = genotype.generateDotFile(true, inputDataSetNames);

    {
        std::ofstream activeStream(activeDotFilePath);
        std::ofstream allStream(allDotFilePath);

        activeStream << activeDotFileText;
        allStream << allDotFileText;
    }

    // TODO: this is probably a security risk.
    std::string cmd = 
        "dot -Tpng " + activeDotFilePath + " -o " + activeImagePath
    +   " && "
    +   "dot -Tpng " + allDotFilePath + " -o " + allImagePath;

    int result = std::system(cmd.c_str());
}

void cc::CheckpointSaver::savePredictions(
        const std::string& outputDirectory,
        const cc::DataChunk& predictions) {
    std::string outputFilePath = outputDirectory + "/predictions.dat";

    std::ofstream stream(outputFilePath, std::ios::out | std::ios::binary);
    const unsigned char* data = predictions.getConstCharDataPtr();
    int size = predictions.getSizeInBytes();

    stream.write(reinterpret_cast<const char*>(data), size);
}

void cc::CheckpointSaver::parseParameters(
        const std::unordered_map<std::string, std::string>& checkpointSaverParameters) {
    outputRootDirectory_ = checkpointSaverParameters.at("outputDirectory");
    generateCode_ = (
        !strncasecmp(
            "true", 
            checkpointSaverParameters.at("generateCode").c_str(), 
            5));
    generateImage_ = (
        !strncasecmp(
            "true", 
            checkpointSaverParameters.at("generateImage").c_str(), 
            5));
    generatePredictions_ = (
        !strncasecmp(
            "true", 
            checkpointSaverParameters.at("generatePredictions").c_str(), 
            5));

}
