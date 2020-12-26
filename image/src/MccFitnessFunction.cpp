#include "../include/MccFitnessFunction.hpp"

#include <cmath>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

double ci::MccFitnessFunction::evaluate(
    const cc::DataChunk& predictions,
    const cc::DataChunk& labels,
    const cc::Genotype& genotype) {

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
