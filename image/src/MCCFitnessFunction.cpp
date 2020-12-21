#include "../include/MCCFitnessFunction.hpp"

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

double ci::MCCFitnessFunction::evaluate(
    const cc::DataChunk& predictions,
    const cc::DataChunk& labels,
    const cc::Genotype& genotype) {

    // TODO: actually implement.
    return 0.0;
}
