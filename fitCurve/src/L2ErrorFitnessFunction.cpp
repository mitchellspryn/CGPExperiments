#include "../include/L2ErrorFitnessFunction.hpp"

#include <cmath>
#include <assert.h>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

double fc::L2ErrorFitnessFunction::evaluate(
        const cc::DataChunk& predictions,
        const cc::DataChunk& labels,
        const cc::Genotype& genotype) {
    assert(predictions.getWidth() == 0);
    assert(predictions.getHeight() == 0);
    assert(labels.getWidth() == 0);
    assert(labels.getHeight() == 0);
    assert(predictions.getNum() == labels.getNum());

    float err = 0;

    const float* p = predictions.getConstDataPtr();
    const float* l = labels.getConstDataPtr();

    for (int i = 0; i < predictions.getNum(); i++) {
        err += (p[i]-l[i]) * (p[i]-l[i]);
    }
    
    return std::sqrt(err);
}
