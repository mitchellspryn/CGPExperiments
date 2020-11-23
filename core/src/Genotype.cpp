#include "../include/Genotype.hpp"

namespace cc = cgpExperiments::core;

cc::Genotype::Genotype(
        const std::unordered_map<std::string, std::string>& genotypeParameters,
        std::shared_ptr<GeneFactory> geneFactory,
        std::shared_ptr<FitnessFunction> fitnessFunction) {
}

void cc::Genotype::Mutate(cc::RandomNumberGenerator& rng) {
}

const cc::DataChunk& cc::Genotype::evaluate(int sampleStartIndex) {

}

std::vector<std::unordered_set<std::string, std::string>> cc::Genotype::serialize() const {
    std::vector<std::unordered_set<std::string, std::string>> output;
}

std::string cc::Genotype::generateCode(cc::CodeGenerationContext_t& context) {

}
