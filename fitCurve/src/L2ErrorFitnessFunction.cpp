#include "../include/L2ErrorFitnessFunction.hpp"

#include <cmath>
#include <assert.h>

namespace cc = cgpExperiments::core;
namespace cf = cgpExperiments::fitCurve;

double cf::L2ErrorFitnessFunction::evaluate(
        const cc::DataChunk& predictions,
        const cc::DataChunk& labels,
        const cc::Genotype& genotype) {
    assert(predictions.getWidth() == 0);
    assert(predictions.getHeight() == 0);
    assert(labels.getWidth() == 0);
    assert(labels.getHeight() == 0);
    assert(predictions.getNum() == labels.getNum());

    float err = 0;

    for (int i = 0; i < predictions.getNum(); i++) {
        float p = predictions.getVal(i, 0, 0);
        float l = labels.getVal(i, 0, 0);

        err += (p-l)*(p-l);
    }
    
    return std::sqrt(err);
}
