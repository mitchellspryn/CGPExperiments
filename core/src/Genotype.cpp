#include "../include/Genotype.hpp"

#include <iostream>
#include <stdexcept>
#include <strings.h>
#include <sstream>
#include <queue>

namespace cc = cgpExperiments::core;

cc::Genotype::Genotype(
        std::shared_ptr<cc::ExperimentConfiguration> experimentConfiguration,
        std::shared_ptr<cc::GenePool> genePool,
        std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator) {
    genePool_ = genePool;
    experimentConfiguration_ = experimentConfiguration;
    randomNumberGenerator_ = randomNumberGenerator;

    fillParametersFromMap(experimentConfiguration_->getGenotypeParameters());
}

void cc::Genotype::mutate() {
    switch (mutationType_) {
        case MutationType::Percentage:
            mutateUntilPercentage();
            break;
        case MutationType::Probability:
            mutateByProbability();
            break;
        case MutationType::SingleActive:
            mutateUntilSingleActive();
            break;
        default:
            throw std::runtime_error("Unrecognized mutationType.");
    }
}

void cc::Genotype::setGenes(
        const std::vector<std::unique_ptr<cc::Gene>>& modelGenes,
        const std::unordered_set<int>& activeGenes) {
    for (size_t i = 0; i < genes_.size(); i++) {
        genePool_->returnToPool(std::move(genes_[i]));
        genes_[i] = genePool_->getFromPool(modelGenes[i]->getTypeId());
        genes_[i]->setRandomNumberGenerator(randomNumberGenerator_.get());
        genes_[i]->initializeFromTemplateGene(modelGenes[i].get());
    }

    activeGenes_ = activeGenes;
}

const std::vector<std::unique_ptr<cc::Gene>>& cc::Genotype::getGenes() const {
    return genes_; 
}

const std::unordered_set<int> cc::Genotype::getActiveGenes() const {
    return activeGenes_;
}

const cc::DataChunk& cc::Genotype::evaluate(std::vector<std::shared_ptr<DataChunk>>& inputs) {
    if (inputs.size() != numInputDatasets_) {
        throw std::runtime_error(
            "Attempted to evaluate function with "
            + std::to_string(inputs.size())
            + " inputs, but expected "
            + std::to_string(numInputDatasets_)
            + " inputs.");
    }

    for (int i = 0; i < numInputDatasets_; i++) {
        buffers_[i] = inputs[i];
    }

    activeGenes_.clear();

    std::queue<int> queue;
    queue.push(outputBufferIndex_);

    while (!queue.empty()) {
        int nextIndex = queue.front();
        queue.pop();
        if ((nextIndex >= numInputDatasets_)) {
            indicesToEvaluate_.push(nextIndex);
            
            const std::vector<int>& dependencies = 
                genes_[nextIndex-numInputDatasets_]->getInputBufferIndices();

            for (size_t i = 0; i < dependencies.size(); i++) {
                queue.push(dependencies[i]);
            }
        }
    }

    while (!indicesToEvaluate_.empty()) {
        int geneIndexToEvaluate = indicesToEvaluate_.top() - numInputDatasets_;
        indicesToEvaluate_.pop();
        
        if (activeGenes_.count(geneIndexToEvaluate) == 0) {
            genes_[geneIndexToEvaluate]->evaluate(buffers_);
            activeGenes_.emplace(geneIndexToEvaluate);
        }
    }

    return *(buffers_[outputBufferIndex_]);
}

std::vector<std::unordered_map<std::string, std::string>> cc::Genotype::serialize() const {
    std::vector<std::unordered_map<std::string, std::string>> result;
    result.reserve((genes_.size()) + 1);

    std::unordered_map<std::string, std::string> genotypeParameters;
    genotypeParameters["geneGridWidth"] = std::to_string(geneGridWidth_);
    genotypeParameters["geneGridHeight"] = std::to_string(geneGridHeight_);
    genotypeParameters["maxLookback"] = std::to_string(maxLookback_);
    genotypeParameters["numInputDatasets"] = std::to_string(numInputDatasets_);
    genotypeParameters["inputDataWidth"] = std::to_string(inputDataWidth_);
    genotypeParameters["inputDataHeight"] = std::to_string(inputDataHeight_);
    genotypeParameters["inputDataNumSamples"] = std::to_string(inputDataNumSamples_);

    switch (mutationType_) {
        case MutationType::Percentage: 
            genotypeParameters["mutationType"] = "percentage";
            genotypeParameters["mutationPercentage"] = std::to_string(mutationPercentage_);
            break;
        case MutationType::Probability:
            genotypeParameters["mutationType"] = "probability";
            genotypeParameters["mutationProbability"] = std::to_string(mutationProbability_);
            break;
        case MutationType::SingleActive:
            genotypeParameters["mutationType"] = "singleActive";
            break;
        default:
            throw std::runtime_error("Unrecognized mutation type.");
    }

    result.push_back(genotypeParameters);

    for (size_t i = 0; i < genes_.size(); i++) {
        result.push_back(genes_[i]->serialize()); 
    }

    return result;
}

void cc::Genotype::deserialize(const std::vector<std::unordered_map<std::string, std::string>>& data) {
    if (data.size() == 0) {
        throw std::runtime_error("Attempted to deserialize an empty genotype.");
    }   

    std::unordered_map<std::string, std::string> genomeParameters = data[0]; 
    fillParametersFromMap(genomeParameters);

    int expectedNumberOfGenes = geneGridWidth_ * geneGridHeight_;
    if (data.size() != expectedNumberOfGenes + 1) {
        throw std::runtime_error(
                "Unexpected genome size when deserializing. Expected "
                + std::to_string(expectedNumberOfGenes+1)
                + ", but got "
                + std::to_string(data.size()));
    }

    for (int i = 1; i < data.size(); i++) {
        int geneTypeId = std::stoi(data.at(i).at("geneTypeId"));
        genes_[i-1] = genePool_->getFromPool(geneTypeId);
        genes_[i-1]->setRandomNumberGenerator(randomNumberGenerator_.get());
        genes_[i-1]->initializeFromParameters(data.at(i));

        // connect inputs
        std::stringstream stream(data.at(i).at("inputBufferIndexes"));
        int inputNumber = 0;
        while (stream.good()) {
            std::string indexStr;
            std::getline(stream, indexStr, ',');
            int index = std::stoi(indexStr);
            genes_[i-1]->connectInput(inputNumber, index);
            inputNumber++;
        }

        genes_[i-1]->setOutputIndex(numInputDatasets_ + i - 1);
    }
}

// TODO: elimination of dead nodes.
std::string cc::Genotype::generateCode(cc::CodeGenerationContext_t& context) const {
    if (context.generationLanguage != GenerationLanguage::CPP) {
        throw std::runtime_error("Only c++ code generation is currently supported.");
    }

    // Generate the function
    std::stringstream functionStringStream;
    functionStringStream << "    void compute(";

    context.variableNamesInUse.clear();
    std::unordered_set<std::string> inputVariableNames;

    for (int i = 0; i < numInputDatasets_; i++) {
        std::string inputVariableName = "in" + std::to_string(i);
        inputVariableNames.emplace(inputVariableName);
        functionStringStream << "const float* " << inputVariableName;
        context.variableNamesInUse.emplace(inputVariableName);
        functionStringStream << ", ";
    }

    functionStringStream << "float* outputBuf, int width, int height, int num) {\n" ;
    context.variableNamesInUse.emplace("width");
    context.variableNamesInUse.emplace("height");
    context.variableNamesInUse.emplace("num");

    std::stack<int> indexesToGenerate;
    std::queue<int> queue;
    queue.push(outputBufferIndex_);

    while (!queue.empty()) {
        int nextIndex = queue.front();
        queue.pop();
        if ((nextIndex >= numInputDatasets_)) {
            indexesToGenerate.push(nextIndex);
            
            const std::vector<int>& dependencies = 
                genes_[nextIndex-numInputDatasets_]->getInputBufferIndices();

            for (size_t i = 0; i < dependencies.size(); i++) {
                queue.push(dependencies[i]);
            }
        }
    }

    std::unordered_set<int> generatedIndexes;
    while (!indexesToGenerate.empty()) {
        int geneIndexToGenerate = indexesToGenerate.top() - numInputDatasets_;
        indexesToGenerate.pop();

        if (generatedIndexes.count(geneIndexToGenerate) != 0) {
            continue;
        }
        
        context.inputVariableNames.clear();
        context.outputVariableName = "tmp" + std::to_string(geneIndexToGenerate);
        context.variableNamesInUse.emplace(context.outputVariableName);

        const std::vector<int>& geneBufferIndices = genes_[geneIndexToGenerate]->getInputBufferIndices();
        for (size_t i = 0; i < geneBufferIndices.size(); i++) {
            int geneBufferIndex = geneBufferIndices[i];
            if (geneBufferIndex < numInputDatasets_) {
                context.inputVariableNames.emplace_back("in" + std::to_string(geneBufferIndex));
            } else {
                context.inputVariableNames.emplace_back("tmp" + std::to_string(geneBufferIndex - numInputDatasets_));
            }
        }

        auto replaceAllFxn = [](std::string str, const std::string& from, const std::string& to) -> std::string {
            size_t startPos = 0;
            while((startPos = str.find(from, startPos)) != std::string::npos) {
                str.replace(startPos, from.length(), to);
                startPos += to.length(); // Handles case where 'to' is a substring of 'from'
            }
            return str;
        }; 

        functionStringStream << "      ////////////////////////////////////////////////////\n";
        functionStringStream << "      //" << genes_[geneIndexToGenerate]->getGeneName() << ": " << geneIndexToGenerate << "\n";
        functionStringStream << "      ////////////////////////////////////////////////////\n";
        std::string generatedSnippet = genes_[geneIndexToGenerate]->generateCode(context);
        functionStringStream << "      " << replaceAllFxn(generatedSnippet, "\n", "\n      ");
        functionStringStream << "\n";

        generatedIndexes.emplace(geneIndexToGenerate);
    }
    
    functionStringStream << "    ////////////////////////////////////////////////////\n";
    functionStringStream 
        << 
        "    return tmp" 
        << std::to_string(outputBufferIndex_ - numInputDatasets_)
        << ".data();\n  }\n";

    std::stringstream constructorStringStream;
    constructorStringStream << "    GeneratedFunction() {\n";

    std::stringstream variableDeclarationStringStream;
    
    int numFloats = buffers_[0]->getSize();
    for (const std::string& variable : context.variableNamesInUse) {
        if ((variable != "width")
            && (variable != "height")
            && (variable != "num")
            && (inputVariableNames.count(variable) == 0)) {
            constructorStringStream 
                << "      " 
                << variable 
                << ".resize(" 
                << std::to_string(numFloats)
                << ", 0);\n";

            variableDeclarationStringStream << "    std::vector<float> " << variable << ";\n";
        }
    }

    constructorStringStream << "    }\n";

    std::stringstream outputStream;
    outputStream << ""
    <<   "#include <cmath>\n"
    <<   "#include <vector>\n"
    <<   "\n"
    <<   "class GeneratedFunction {\n" 
    <<   "  public:\n"
    <<   constructorStringStream.str()
    <<   "\n"
    <<   functionStringStream.str()
    <<   "\n"
    <<   "  private:\n"
    <<   variableDeclarationStringStream.str()
    <<   "\n"
    <<   "};";

    return outputStream.str();
}

std::string cc::Genotype::generateDotFile(bool includeUnusedNodes) const {
    std::stringstream outputStream;
    outputStream << "digraph graphname {\n";
    outputStream << "  size=\"1000, 1000\";\n";

    for (size_t i = 0; i < numInputDatasets_; i++) {
        outputStream 
            << "  "
            << std::to_string(i)
            << " [label=\"in:"
            << std::to_string(i)
            << "\", shape=\"ellipse\", style=\"filled\", fillcolor=\"azure\"];\n";
    }

    for (size_t i = 0; i < genes_.size(); i++) {
        bool isActive = (activeGenes_.count(i) > 0);

        if (includeUnusedNodes || isActive) {
            std::string color = "beige";
            if (isActive) {
                if (i == outputBufferIndex_ - numInputDatasets_) {
                    color = "darkgreen";
                } else {
                    color = "bisque2";
                }
            }

            outputStream
                << "  "
                << std::to_string(i + numInputDatasets_)
                << " [label=\""
                << genes_[i]->getGeneName()
                << ": "
                << i
                << "\", shape=\"box\", style=\"filled\", fillcolor=\""
                << color
                << "\"];\n";

            const std::vector<int>& inputs = genes_[i]->getInputBufferIndices();
            for (size_t j = 0; j < inputs.size(); j++) {
                outputStream 
                    << "  "
                    << std::to_string(inputs[j])
                    << " -> "
                    << std::to_string(i + numInputDatasets_)
                    << ";\n";
            }
        }
    }

    outputStream << "}";
    
    return outputStream.str();
}

void cc::Genotype::initializeRandomly() {
    for (size_t i = 0; i < genes_.size(); i++) {
        genes_[i] = genePool_->getRandomGeneFromPool();
        genes_[i]->setRandomNumberGenerator(randomNumberGenerator_.get());
        genes_[i]->initializeFromParameters(
            experimentConfiguration_->getGeneParameters(
                genes_[i]->getGeneName()));
        genes_[i]->setOutputIndex(numInputDatasets_ + i);
        for (int j = 0; j < genes_[i]->getNumInputs(); j++) {
            randomlyReconnectGeneInput(j, i);
        }
    }

    outputBufferIndex_ = 
        numInputDatasets_ + randomNumberGenerator_->getRandomInt(0, genes_.size() - 1);
}

void cc::Genotype::randomlyReconnectGeneInput(int inputNumber, int geneIndex) {
    // Compute the candidate range
    if (geneIndex == 0) {
        int k = 0;
    }
    int endColumn = std::max(0, ((geneIndex / geneGridHeight_) - 1));
    int startColumn = std::max(0, endColumn - maxLookback_);

    int columnNumber = randomNumberGenerator_->getRandomInt(startColumn, endColumn);

    int connectIndex = 0;
    if (columnNumber == 0) {
        connectIndex = randomNumberGenerator_->getRandomInt(0, numInputDatasets_ - 1); 
    } else {
        connectIndex = 
            (randomNumberGenerator_->getRandomInt(0, geneGridHeight_ - 1))
            + (columnNumber * geneGridHeight_)
            + numInputDatasets_;
    }

    genes_[geneIndex]->connectInput(inputNumber, connectIndex);
}

void cc::Genotype::mutateUntilPercentage() {
    std::unordered_set<int> mutatedGenes;
    while (mutatedGenes.size() < mutationPercentageNumGenes_) {
        int geneIndex = randomNumberGenerator_->getRandomInt(0, genes_.size() - 1);
        if (mutatedGenes.count(geneIndex) == 0) {
            mutateSingleGene(geneIndex);
            mutatedGenes.emplace(geneIndex);
        }
    }
}

void cc::Genotype::mutateByProbability() {
    for (size_t i = 0; i <= genes_.size(); i++) {
        if (randomNumberGenerator_->getRandomFloat() < mutationProbability_) {
            mutateSingleGene(i);
        }
    }
}

void cc::Genotype::mutateUntilSingleActive() {
    bool mutatedActiveGene = false;
    while (!mutatedActiveGene) {
        int geneIndex = randomNumberGenerator_->getRandomInt(0, genes_.size() - 1);
        mutateSingleGene(geneIndex);
        mutatedActiveGene = (
                (geneIndex == genes_.size())
                || (activeGenes_.count(geneIndex) != 0));
    }
}

void cc::Genotype::mutateSingleGene(int geneIndex) {
    // Special case - change the output to point somewhere else
    if (geneIndex == genes_.size()) {
        outputBufferIndex_ = 
            numInputDatasets_ + randomNumberGenerator_->getRandomInt(0, genes_.size() - 1);
        return;
    }

    // 0 => reconnect an input
    // 1 => completely new random gene
    // 2 => mutate a parameter
    int upperBound = 2;
    int lowerBound = 0;

    // TODO: is this a good idea?
    //// Do not mutate a gene's parameters if it has none.
    //if (genes_[geneIndex]->isParameterFree()) {
    //    upperBound = 1;
    //}

    //// Do not try to reconnect an input if it has none (e.g. constant node)
    //if (genes_[geneIndex]->getNumInputs() == 0) {
    //    lowerBound = 1;
    //}

    int typeOfMutation = randomNumberGenerator_->getRandomInt(lowerBound, upperBound);
    
    if (typeOfMutation == 0) {
        if (genes_[geneIndex]->getNumInputs() > 0) {
            int inputToReconnect = 
                randomNumberGenerator_->getRandomInt(0, genes_[geneIndex]->getNumInputs() - 1);
            randomlyReconnectGeneInput(inputToReconnect, geneIndex);
        }
    } else if (typeOfMutation == 1) {
        genes_[geneIndex] = genePool_->getRandomGeneFromPool();
        genes_[geneIndex]->setRandomNumberGenerator(randomNumberGenerator_.get());
        genes_[geneIndex]->initializeFromParameters(
            experimentConfiguration_->getGeneParameters(
                genes_[geneIndex]->getGeneName()));
        genes_[geneIndex]->setOutputIndex(numInputDatasets_ + geneIndex);

        int numInputs = genes_[geneIndex]->getNumInputs();
        for (int i = 0; i < numInputs; i++) {
            randomlyReconnectGeneInput(i, geneIndex);
        }
    } else if (typeOfMutation == 2) {
        genes_[geneIndex]->mutateParameters();
    }
}

void cc::Genotype::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& params) {
    geneGridWidth_ = std::stoi(params.at("geneGridWidth"));
    geneGridHeight_ = std::stoi(params.at("geneGridHeight"));
    maxLookback_ = std::stoi(params.at("maxLookback"));
    numInputDatasets_ = std::stoi(params.at("numInputDatasets"));
    inputDataWidth_ = std::stoi(params.at("inputDataWidth"));
    inputDataHeight_ = std::stoi(params.at("inputDataHeight"));
    inputDataNumSamples_ = std::stoi(params.at("inputDataNumSamples"));

    if (strncasecmp(params.at("mutationType").c_str(), "percentage", 20) == 0) {
        mutationType_ = MutationType::Percentage;
        mutationPercentage_ = std::stod(params.at("mutationPercentage"));
        if (mutationPercentage_ >= 1.0 || mutationPercentage_ <= 0) {
            throw std::runtime_error(
                    "Invalid mutation percentage: " 
                    + std::to_string(mutationPercentage_)
                    + ". Expected number on the range (0, 1).");
        }
        mutationPercentageNumGenes_ = std::ceil(genes_.size() * mutationPercentage_);
    } else if (strncasecmp(params.at("mutationType").c_str(), "probability", 20) == 0) {
        mutationType_ = MutationType::Probability;
        mutationProbability_ = std::stod(params.at("mutationProbability"));
        if (mutationProbability_ >= 1.0 || mutationProbability_ < 0) {
            throw std::runtime_error(
                    "Invalid mutation probability: "
                    + std::to_string(mutationProbability_)
                    + ". Expected number on the range (0, 1).");
        }
    } else if (strncasecmp(params.at("mutationType").c_str(), "singleActive", 20) == 0) { 
        mutationType_ = MutationType::SingleActive;
    } else {
        throw std::runtime_error(
                "Invalid mutation type: " 
                + params.at("mutationType")
                + ".");
    }

    genes_.reserve(geneGridWidth_ * geneGridHeight_);
    for (size_t i = 0; i < (geneGridWidth_ * geneGridHeight_); i++) {
        std::unique_ptr<cc::Gene> gp(nullptr);

        genes_.emplace_back(std::move(gp));
    }

    buffers_.reserve(genes_.size() + numInputDatasets_);
    for (int i = 0; i < genes_.size() + numInputDatasets_; i++) {
        std::unique_ptr<cc::DataChunk> dc = 
            std::make_unique<cc::DataChunk>(
                inputDataWidth_, 
                inputDataHeight_, 
                inputDataNumSamples_);

        buffers_.emplace_back(
            std::make_unique<cc::DataChunk>(
                inputDataWidth_, inputDataHeight_, inputDataNumSamples_));
    }

    if (params.count("outputIndex") > 0) {
        outputBufferIndex_ = std::stoi(params.at("outputIndex"));
    } else {
        outputBufferIndex_ = randomNumberGenerator_->getRandomInt(
                numInputDatasets_,
                numInputDatasets_ + genes_.size() - 1);
    }
}

