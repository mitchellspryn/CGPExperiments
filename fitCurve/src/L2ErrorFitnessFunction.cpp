#include "../include/L2ErrorFitnessFunction.hpp"

#include <cmath>
#include <assert.h>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

double fc::L2ErrorFitnessFunction::evaluate(
        const cc::DataChunk& predictions,
        const cc::DataChunk& labels,
        const cc::Genotype& genotype) {
    assert(predictions.getWidth() == 1);
    assert(predictions.getHeight() == 1);
    assert(labels.getWidth() == 1);
    assert(labels.getHeight() == 1);
    assert(predictions.getNum() == labels.getNum());

    float err = 0;

    const float* p = predictions.getConstFloatDataPtr();
    const float* l = labels.getConstFloatDataPtr();

    for (int i = 0; i < predictions.getNum(); i++) {
        err += (p[i]-l[i]) * (p[i]-l[i]);
    }

    return err / static_cast<double>(predictions.getNum());
}
