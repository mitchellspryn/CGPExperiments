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
        int typeId) {
    switch (typeId) {
        case kAddGeneTypeId:
            return std::make_unique<fc::AddGene>();
        case kConstantOutputGeneTypeId:
            return std::make_unique<fc::ConstantOutputGene>();
        case kCosGeneTypeId:
            return std::make_unique<fc::CosGene>();
        case kDivideProtectedGeneTypeId:
            return std::make_unique<fc::DivideProtectedGene>();
        case kMultiplyGeneTypeId:
            return std::make_unique<fc::MultiplyGene>();
        case kSinGeneTypeId:
            return std::make_unique<fc::SinGene>();
        case kSubtractGeneTypeId:
            return std::make_unique<fc::SubtractGene>();
        default:
            break;
    }

    throw std::runtime_error(
        "Unrecognized gene id: '"
        + std::to_string(typeId)
        + "';");
}

int fc::FitCurveGeneFactory::getTypeId(
        const std::string& geneName) {

    const char* name = geneName.c_str();
    if (strncasecmp(name, "add", 4) == 0) {
        return kAddGeneTypeId;
    } else if (strncasecmp(name, "constantOutput", 15) == 0) {
        return kConstantOutputGeneTypeId;
    } else if (strncasecmp(name, "cos", 4) == 0) {
        return kCosGeneTypeId;
    } else if (strncasecmp(name, "divideProtected", 16) == 0) {
        return kDivideProtectedGeneTypeId;
    } else if (strncasecmp(name, "multiply", 9) == 0) {
        return kMultiplyGeneTypeId;
    } else if (strncasecmp(name, "sin", 4) == 0) {
        return kSinGeneTypeId;
    } else if (strncasecmp(name, "subtract", 9) == 0) {
        return kSubtractGeneTypeId;
    }

    throw std::runtime_error(
        "Unrecognized gene name: '"
        + geneName
        + "';");
}
