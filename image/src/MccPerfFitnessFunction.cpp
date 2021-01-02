#include "../include/MccPerfFitnessFunction.hpp"

#include <cmath>
#include <memory>
#include <vector>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

ci::MccPerfFitnessFunction::MccPerfFitnessFunction(double lambda) {
    lambda_ = lambda;
}

ci::MccPerfFitnessFunction::MccPerfFitnessFunction(
        const std::unordered_map<std::string, std::string>& parameters) {
    lambda_ = std::stod(parameters.at("lambda"));
}

double ci::MccPerfFitnessFunction::evaluate(
        const cc::DataChunk& predictions,
        const cc::DataChunk& labels,
        const cc::Genotype& genotype) {
    double mccLoss = computeMccLoss(predictions, labels);
    double perfLoss = computePerfLoss(genotype);

    return mccLoss + (lambda_ * perfLoss);
}

double ci::MccPerfFitnessFunction::computeMccLoss(
        const cc::DataChunk& predictions,
        const cc::DataChunk& labels) {

    // (True/False) (Positive/Negative)
    long long tp = 0;
    long long tn = 0;
    long long fp = 0;
    long long fn = 0;

    int numBytes = predictions.getSizeInBytes();
    const unsigned char* predictionData = predictions.getConstCharDataPtr();
    const unsigned char* labelData = labels.getConstCharDataPtr();

    constexpr unsigned char threshold = 128;
    for (int i = 0; i < numBytes; i++) {
        const unsigned char p = predictionData[i];
        const unsigned char l = labelData[i];

        if (p >= threshold) {
            if (l >= threshold) {
                tp++;
            } else {
                fp++;
            }
        } else {
            if (l >= threshold) {
                fn++;
            } else {
                tn++;
            }
        }
    }

    double num = static_cast<double>(tp*tn) - static_cast<double>(fp*fn);
    double denom = static_cast<double>(tp+fp)
                    * static_cast<double>(tp+fn)
                    * static_cast<double>(tn+fp)
                    * static_cast<double>(tn+fn);
    
    if ((num == 0) || (denom == 0)) {
        return 1;
    }

    double mcc = num / std::sqrt(denom);

    // TODO: should we use std::abs?
    double fitness = 1.0 - std::abs(mcc);

    return fitness;
}

double ci::MccPerfFitnessFunction::computePerfLoss(const cc::Genotype& genotype) {
    const std::vector<std::unique_ptr<cc::Gene>>& genes = genotype.getGenes();
    const std::unordered_set<int>& activeGenes = genotype.getActiveGenes();

    double cost = 0;
    for (const int index : activeGenes) {
        cost += genes[index]->getComputeCost();
    }

    return cost;
}
