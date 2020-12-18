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
        case fc::kAddGeneTypeId:
            return std::make_unique<fc::AddGene>();
        case fc::kConstantOutputGeneTypeId:
            return std::make_unique<fc::ConstantOutputGene>();
        case fc::kCosGeneTypeId:
            return std::make_unique<fc::CosGene>();
        case fc::kDivideProtectedGeneTypeId:
            return std::make_unique<fc::DivideProtectedGene>();
        case fc::kMultiplyGeneTypeId:
            return std::make_unique<fc::MultiplyGene>();
        case fc::kSinGeneTypeId:
            return std::make_unique<fc::SinGene>();
        case fc::kSubtractGeneTypeId:
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

    if (strncasecmp(name, fc::kAddGeneTypeName, 4) == 0) {
        return fc::kAddGeneTypeId;
    } else if (strncasecmp(name, fc::kConstantOutputGeneTypeName, 15) == 0) {
        return fc::kConstantOutputGeneTypeId;
    } else if (strncasecmp(name, fc::kCosGeneTypeName, 4) == 0) {
        return fc::kCosGeneTypeId;
    } else if (strncasecmp(name, fc::kDivideProtectedGeneTypeName, 16) == 0) {
        return fc::kDivideProtectedGeneTypeId;
    } else if (strncasecmp(name, fc::kMultiplyGeneTypeName, 9) == 0) {
        return fc::kMultiplyGeneTypeId;
    } else if (strncasecmp(name, fc::kSinGeneTypeName, 4) == 0) {
        return fc::kSinGeneTypeId;
    } else if (strncasecmp(name, fc::kSubtractGeneTypeName, 9) == 0) {
        return fc::kSubtractGeneTypeId;
    }

    throw std::runtime_error(
        "Unrecognized gene name: '"
        + geneName
        + "';");
}
