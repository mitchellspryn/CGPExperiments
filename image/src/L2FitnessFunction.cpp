#include "../include/L2FitnessFunction.hpp"

#include <cmath>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

double ci::L2FitnessFunction::evaluate(
    const cc::DataChunk& predictions,
    const cc::DataChunk& labels,
    const cc::Genotype& genotype) {

    int numBytes = predictions.getSizeInBytes();
    const unsigned char* predictionData = predictions.getConstCharDataPtr();
    const unsigned char* labelData = labels.getConstCharDataPtr();

    double accumulator = 0;
    for (int i = 0; i < numBytes; i++) {
        double p = static_cast<double>(predictionData[i]);
        double l = static_cast<double>(labelData[i]);
        accumulator += (p-l) * (p-l);
    }

    accumulator = std::sqrt(accumulator) / static_cast<double>(numBytes);

    return accumulator;
}
