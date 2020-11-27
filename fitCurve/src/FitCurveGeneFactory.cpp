#include "../include/FitCurveGeneFactory.hpp"

#include "../include/genes/AddGene.hpp"
#include "../include/genes/ConstantOutputGene.hpp"
#include "../include/genes/CosGene.hpp"
#include "../include/genes/DivideProtectedGene.hpp"
#include "../include/genes/MultiplyGene.hpp"
#include "../include/genes/SinGene.hpp"
#include "../include/genes/SubtractGene.hpp"

#include <stdexcept>
#include <strings.h>

namespace cc = cgpExperiments::core;
namespace fc = cgpExperiments::fitCurve;

std::unique_ptr<cc::Gene> fc::FitCurveGeneFactory::createGene(
        const std::string& geneName) {
    const char* name = geneName.c_str();
    if (strncasecmp(name, "add", 4) == 0) {
        return std::make_unique<cf::AddGene>();
    } else if (strncasecmp(name, "constantOutput", 15) == 0) {
        return std::make_unique<cf::ConstantOutputGene>();
    } else if (strncasecmp(name, "cos", 4) == 0) {
        return std::make_unique<cf::CosGene>();
    } else if (strncasecmp(name, "divideProtected", 16) == 0) {
        return std::make_unique<cf::DivideProtectedGene>();
    } else if (strncasecmp(name, "multiply", 9) == 0) {
        return std::make_unique<cf::MultiplyGene>();
    } else if (strncasecmp(name, "sin", 4) == 0) {
        return std::make_unique<cf::SinGene>();
    } else if (strncasecmp(name, "subtract", 9) == 0) {
        return std::make_unique<cf::SubtractGene>();
    }

    throw std::runtime_error(
        "Unrecognized gene name: '"
        + geneName
        + "';");

}
