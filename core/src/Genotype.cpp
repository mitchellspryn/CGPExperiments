#include "../include/Genotype.hpp"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <queue>

namespace cc = cgpExperiments::core;

cc::Genotype::Genotype(
        std::shared_ptr<ExperimentConfiguration> experimentConfiguration,
        std::shared_ptr<GenePool> genePool) {
    genePool_ = genePool;
    experimentConfiguration_ = experimentConfiguration;

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
    // TODO: This is a bit sketchy, as we'll be constantly cycling through allocations.
    // Potentially, we could solve this with a pooling implementation 
    //      (gene pool. heh)
    // However, the pool would have to be thread safe, which would hurt parallelism.
    // Elide it for now.
    const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& geneParams = 
    for (size_t i = 0; i < genes_.size(); i++) {
        genePool_->returnToPool(std::move(genes_[i]));
        genes_[i] = genePool_->getFromPool(modelGenes[i]->getGeneName());
        genes_[i]->initializeFromTemplateGene(modelGenes[i].get());
    }

    activeGenes_ = activeGenes;
}

const std::vector<std::unique_ptr<cc::Gene>>& cc::Genotype::getGenes() {
    return genes_; 
}

const std::unordered_set<int> cc::Genotype::getActiveGenes() {
    return activeGenes_;
}

const cc::DataChunk& cc::Genotype::evaluate(std::vector<std::shared_ptr<DataChunk>>& inputs) {
    if (inputs.size() != numInputDatasets_) {
        throw std::runtime_error(
            "Attempted to evaluate function with "
            + std::to_string(inputs.size())
            + " inputs, but expected "
            + numInputDatasets_
            + " inputs.");
    }

    for (int i = 0; i < numInputDatasets_; i++) {
        buffers_[i] = inputs[i];
    }

    indicesToEvaluate_.clear();
    activeGenes_.clear();

    std::queue<int> queue;
    queue.push(outputBufferIndex_);

    while (!queue.empty()) {
        int nextIndex = queue.front();
        queue.pop();
        if ((nextIndex >= numInputDatasets_)) {
            indicesToEvaluate_.push(nextIndex);
            
            const std::vector<int>& dependencies = 
                genes_[nextIndex-numInputDatasets_]->getInputBufferIndexes();

            for (size_t i = 0; i < dependencies.size(); i++) {
                queue.push(dependencies[i]);
            }
        }
    }

    while (!indicesToEvaluate_.empty()) {
        int geneIndexToEvaluate = indicesToEvaluate_.top() - numInputDatasets_;
        indicesToEvaluate_.pop();
        
        if (activeGenes.count(geneIndexToEvaluate) != 0) {
            genes_[geneIndexToEvaluate]->evaluate(buffers_);
            activeGenes.emplace(geneIndexToEvaluate);
        }
    }

    return buffers_[outputBufferIndex_];
}

std::vector<std::unordered_set<std::string, std::string>> cc::Genotype::serialize() const {
    std::vector<std::unordered_set<std::string, std::string>> result;
    result.reserve((genes_.size()) + 1);

    std::unordered_map<std::string, std::string> genotypeParameters;
    genotypeParameters["geneGridWidth"] = std::to_string(geneGridWidth_);
    genotypeParameters["geneGridHeight"] = std::to_string(geneGridHeght_);
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

void cc::Genotype::deserialize(const std::vector<std::unordered_set<std::string, std::string>>& data) {
    if (data.size() == 0) {
        throw std::runtime_error("Attempted to deserialize an empty genotype.");
    }   

    std::unordered_set<std::string, std::string> genomeParameters = data[0]; 
    fillParametersFromMap(genomeParameters);

    int expectedNumberOfGenes = geneGridWidth_ * geneGridHeght_;
    if (data.size() != expectedNumberOfGenes + 1) {
        throw std::runtime_error(
                "Unexpected genome size when deserializing. Expected "
                + std::to_string(expectedNumberOfGenes+1)
                + ", but got "
                + data.size());
    }

    for (int i = 1; i < data.size(); i++) {
        const std::string& geneName = genomeParameters[i]["geneName"];
        genes_[i-1] = genePool_->getFromPool(geneName);
        genes_[i-1]->initializeFromParameters(genomeParameters[i]);

        // connect inputs
        std::stringstream stream(genomeParameters[i]["inputBufferIndexes"]);
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
std::string cc::Genotype::generateCode(cc::CodeGenerationContext_t& context) {
    if (context.generationLanguage != GenerationLanguage::CPP) {
        throw std::runtime_error("Only c++ code generation is currently supported.");
    }

    // Generate the function
    std::stringstream functionStringStream;
    functionStringStream << "    void compute(";

    context.variableNamesInUse.clear();
    
    for (int i = 0; i < numInputDatasets_; i++) {
        std::string inputVariableName = "in" + std::to_string(i);
        functionStringStream << "const float* " << inputVariableName;
        context.variableNamesInUse.emplace(inputVariableName);
        functionStringStream << ", ";
    }

    functionStringStream << "float* outputBuf, int width, int height, int num) {" ;
    context.variableNamesInUse.emplace("width");
    context.variableNamesInUse.emplace("height");
    context.variableNamesInUse.emplace("num");

    int tmpBufferSize

    std::stack<int> indexesToGenerate;
    std::queue<int> queue;
    queue.push(outputBufferIndex_);

    while (!queue.empty()) {
        int nextIndex = queue.front();
        queue.pop();
        if ((nextIndex >= numInputDatasets_)) {
            indexesToGenerate.push(nextIndex);
            
            const std::vector<int>& dependencies = 
                genes_[nextIndex-numInputDatasets_]->getInputBufferIndexes();

            for (size_t i = 0; i < dependencies.size(); i++) {
                queue.push(dependencies[i]);
            }
        }
    }

    std::unordered_map<int> generatedIndexes;
    while (!indexesToGenerate.empty()) {
        int geneIndexToGenerate = indicesToEvaluate_.top() - numInputDatasets_;
        indicesToEvaluate_.pop();

        if (generatedIndexes.count(geneIndexToGenerate) != 0) {
            continue;
        }
        
        context.inputVariableNames.clear();
        context.outputVariableName = "tmp" + geneIndexToGenerate;
        context.variableNamesInUse.emplace(context.outputVariableName);

        const std::vector<int>& geneBufferIndices = genes_[geneIndexToGenerate]->getInputBufferIndices();
        for (size_t i = 0; i < geneBufferIndices.size(); i++) {
            int geneBufferIndex = geneBufferIndices[i];
            if (geneBufferIndex < numInputDatasets_) {
                context.inputVariableNames.emplace_back("in" + std::to_string(i));
            } else {
                context.inputVariableNames.emplace_back("tmp" + std::to_string(i-numInputDatasets_));
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

        functionStringStream << "    ////////////////////////////////////////////////////\n";
        functionStringStream << "    //" << genes_[geneIndexToGenerate]->getGeneName() << ": " << geneIndexToGenerate << "\n";
        functionStringStream << "    ////////////////////////////////////////////////////\n";
        std::string generatedSnippet = genes_[geneIndexToGenerate]->generateCode(context);
        functionStringStream << "    " << replaceAllFxn(generatedSnippet, "\n", "\n    ");
        functionStringStream << "\n";

        generatedIndexes.emplace(geneIndexToGenerate);
    }
    
    functionStringStream << "////////////////////////////////////////////////////\n";
    functionStringStream 
        << 
        "return tmp" 
        << std::to_string(outputBufferIndex_ - numInputDatasets_)
        << ".data();\n}\n";

    std::stringstream constructorStringStream;
    constructorStringStream << "GeneratedFunction() {\n";

    std::stringstream variableDeclarationStringStream;
    
    constexpr int numFloats = buffers_[0].getSize();
    for (const std::string& variable : context.variableNamesInUse) {
        constructorStringStream 
            << "    " 
            << variable 
            << ".resize(" 
            << std::to_string(numFloats)
            << ", 0);\n";

        variableDeclarationStringStream << "    std::vector<float> " << variable << ";\n";
    }

    constructorStringStream << "}\n";

    std::string output = ""
    +   "#include <cmath>\n"
    +   "#include <vector>\n"
    +   "\n"
    +   "class GeneratedFunction {\n" 
    +   "  public:\n"
    +   constructorStringStream.str();
    +   "\n"
    +   functionStringStream.str();
    +   "\n"
    +   "  private:"
    +   variableDeclarationStringStream.str()
    +   "\n"
    +   "};";

    return output;
}

void cc::Genotype::initializeRandomly() {
    for (size_t i = 0; i < genes_.size(); i++) {
        genes[i] = genePool_->getRandomGeneFromPool();
        genes[i]->initializeFromParameters(
            experimentConfiguration_->getGeneParameters(
                genes[i]->getGeneName()));
        genes[i]->setOutputIndex(numInputDatasets_ + i);
        for (int j = 0; j < genes[i]->getNumInputs(); j++) {
            randomlyReconnectGeneInput(j, i);
        }
    }

    outputIndex = 
        numInputDatasets_ + cc::RandomNumberGenerator::getRandomInt(0, genes_.size() - 1);
}

void cc::Genotype::randomlyReconnectGeneInput(int inputNumber, int geneIndex) {
    // Compute the candidate range
    int endColumn = (geneIndex / geneGridHeight_) - 1;
    int startColumn = std::max(0, endColumn - maxLookback_);

    int columnNumber = cc::RandomNumberGenerator::getRandomInt(endColumn, startColumn);

    int connectIndex = 0;
    if (columnNumber == 0) {
        connectIndex = cc::RandomNumberGenerator::getRandomInt(0, numInputDatasets_); 
    } else {
        connectIndex = (cc::RandomNumberGenerator::getRandomInt(0, endColumn-1) * geneGridHeight_)
            + (columnNumber * geneGridHeght)
            + numInputDatasets_;
    }

    genes_[geneIndex]->connectInput(inputNumber, connectIndex);
}

void cc::Genotype::mutateUntilPercentage() {
    std::unordered_set<int> mutatedGenes;
    while (mutatedGenes.size() < mutationPercentageNumGenes_) {
        int geneIndex = cc::RandomNumberGenerator::getRandomInt(0, genes_.size());
        if (mutatedGenes.count(geneIndex) == 0) {
            mutateSingleGene(geneIndex);
            mutatedGenes.add(geneIndex);
        }
    }
}

void cc::Genotype::mutateByProbability() {
    for (size_t i = 0; i <= genes_.size(); i++) {
        if (cc::RandomNumberGenerator::getRandomFloat() < mutationProbability_) {
            mutateSingleGene(i);
        }
    }
}

void cc::Genotype::mutateUntilSingleActive() {
    bool mutatedActiveGene = false;
    while (!mutatedActiveGene) {
        int geneIndex = cc::RandomNumberGenerator::getRandomInt(0, genes_.size());
        mutateSingleGene(geneIndex);
        mutatedActiveGene = (
                (geneIndex == genes_.size())
                || (activeGenes_.count(geneIndex) != 0));
    }
}

void cc::Genotype::mutateSingleGene(int geneIndex) {
    // Special case - change the output to point somewhere else
    if (geneIndex == genes_.size()) {
        outputIndex_ = 
            numInputDatasets_ + cc::RandomNumberGenerator::getRandomInt(0, genes_.size() - 1);
        return;
    }

    // 0 => reconnect an input
    // 1 => completely new random gene
    // 2 => mutate a parameter
    int upperBound = 2;
    int lowerBound = 0;

    // Do not mutate a gene's parameters if it has none.
    if (genes_[geneIndex]->isParamterFree()) {
        upperBound = 1;
    }

    // Do not try to reconnect an input if it has none (e.g. constant node)
    if (genes_[geneIndex]->getNumInputs() == 0) {
        lowerBound = 1;
    }

    int typeOfMutation = cc::RandomNumberGenerator::getRandomInt(lowerBound, upperBound);
    
    if (typeOfMutation == 0) {
        int inputToReconnect = 
            cc::RandomNumberGenerator::getRandomInt(0, genes_[geneIndex]->getNumInputs());
        randomlyReconnectGeneInput(inputToReconnect, geneIndex);
    } else if (typeOfMutation == 1) {
        genes_[geneIndex] = genePool_->getRandomGeneFromPool();
        genes_[geneIndex]->initializeFromParameters(
            experimentConfiguration_->getGeneParameters(
                genes_[geneIndex]->getGeneName()));
        
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
    geneGridWidth_ = std::stoi(params["geneGridWidth"]);
    geneGridHeght_ = std::stoi(params["geneGridHeight"]);
    maxLookback_ = std::stoi(params["maxLookback"]);
    numInputDatasets_ = std::stoi(params["numInputDatasets"]);
    inputDataWidth_ = std::stoi(params["inputDataWidth"]);
    inputDataHeight_ = std::stoi(params["inputDataHeight"]);
    inputDataNumSamples_ = std::stoi(params["inputDataNumSamples"]);

    if (strncasecmp(params["mutationType"].c_str(), "percentage", 20) == 0) {
        mutationType_ = MutationType::Percentage;
        mutationPercentage_ = std::stod(params["mutationPercentage"]);
        if (mutationPercentage_ >= 1.0 || mutationPercentage_ <= 0) {
            throw std::runtime_error(
                    "Invalid mutation percentage: " 
                    + std::to_string(mutationPercentage_)
                    + ". Expected number on the range (0, 1).");
        }
        mutationPercentageNumGenes_ = std::ceil(genes_.size() * mutationPercentage_);
    } else if (strncasecmp(params["mutationType"].c_str(), "probability", 20) == 0) {
        mutationType_ = MutationType::Probability;
        mutationProbability_ = std::stod(params["mutationProbability"]);
        if (mutationProbability_ >= 1.0 || mutationProbability_ < 0) {
            throw std::runtime_error(
                    "Invalid mutation probability: "
                    + std::to_string(mutationProbability_)
                    + ". Expected number on the range (0, 1).");
        }
    } else if (strncasecmp(params["mutationType"].c_str(), "singleActive", 20) == 0) { 
        mutationType = MutationType::SingleActive;
    } else {
        throw std::runtime_error(
                "Invalid mutation type: " 
                + params["mutationType"]
                + ".");
    }

    if (params.count("outputIndex") > 0) {
        outputIndex_ = std::stoi(params["outputIndex"]);
    }

    genes_.resize(geneGridHeght_ * geneGridWidth_, nullptr);

    buffers_.reserve(genes_.size() + numInputDatasets_);
    for (int i = 0; i < genes_.size() + numInputDatasets_; i++) {
        buffers.emplace_back(inputdataWidth_, inputDataHeight_, inputDataNumSamples_);
    }
}

