#include "../include/ImageGeneFactory.hpp"

#include "../include/genes/AbsDifference.hpp"
#include "../include/genes/AddC.hpp"
#include "../include/genes/Add.hpp"
#include "../include/genes/Average.hpp"
#include "../include/genes/AverageValue.hpp"
#include "../include/genes/Canny.hpp"
#include "../include/genes/ConstantOutput.hpp"
#include "../include/genes/Dilate.hpp"
#include "../include/genes/Erode.hpp"
#include "../include/genes/Exp.hpp"
#include "../include/genes/Gabor.hpp"
#include "../include/genes/GaussianBlur.hpp"
#include "../include/genes/Laplace.hpp"
#include "../include/genes/LocalAverage.hpp"
#include "../include/genes/LocalMax.hpp"
#include "../include/genes/LocalMin.hpp"
#include "../include/genes/LocalNormalize.hpp"
#include "../include/genes/Log.hpp"
#include "../include/genes/MaxC.hpp"
#include "../include/genes/Max.hpp"
#include "../include/genes/MaxValue.hpp"
#include "../include/genes/MinC.hpp"
#include "../include/genes/Min.hpp"
#include "../include/genes/MinValue.hpp"
#include "../include/genes/MultiplyC.hpp"
#include "../include/genes/Multiply.hpp"
#include "../include/genes/Normalize.hpp"
#include "../include/genes/Rescale.hpp"
#include "../include/genes/ResizeThenGabor.hpp"
#include "../include/genes/Shift.hpp"
#include "../include/genes/SmoothBilateral.hpp"
#include "../include/genes/SmoothMedian.hpp"
#include "../include/genes/Sobel.hpp"
#include "../include/genes/SobelX.hpp"
#include "../include/genes/SobelY.hpp"
#include "../include/genes/Sqrt.hpp"
#include "../include/genes/SubtractC.hpp"
#include "../include/genes/Subtract.hpp"
#include "../include/genes/Threshold.hpp"
#include "../include/genes/Unsharpen.hpp"

#include <stdexcept>
#include <strings.h>

namespace cc = cgpExperiments::core;
namespace ci = cgpExperiments::image;

std::unique_ptr<cc::Gene> ci::ImageGeneFactory::createGene(int typeId) {
    switch (typeId) {
        case ci::kAbsDifferenceGeneTypeId:
            return std::make_unique<ci::AbsDifferenceGene>();
        case ci::kAddCGeneTypeId:
            return std::make_unique<ci::AddCGene>();
        case ci::kAddGeneTypeId:
            return std::make_unique<ci::AddGene>();
        case ci::kAverageGeneTypeId:
            return std::make_unique<ci::AverageGene>();
        case ci::kAverageValueGeneTypeId:
            return std::make_unique<ci::AverageValueGene>();
        case ci::kCannyGeneTypeId:
            return std::make_unique<ci::CannyGene>();
        case ci::kConstantOutputGeneTypeId:
            return std::make_unique<ci::ConstantOutputGene>();
        case ci::kDilateGeneTypeId:
            return std::make_unique<ci::DilateGene>();
        case ci::kErodeGeneTypeId:
            return std::make_unique<ci::ErodeGene>();
        case ci::kExpGeneTypeId:
            return std::make_unique<ci::ExpGene>();
        case ci::kGaborGeneTypeId:
            return std::make_unique<ci::GaborGene>();
        case ci::kGaussianBlurGeneTypeId:
            return std::make_unique<ci::GaussianBlurGene>();
        case ci::kLaplaceGeneTypeId:
            return std::make_unique<ci::LaplaceGene>();
        case ci::kLocalAverageGeneTypeId:
            return std::make_unique<ci::LocalAverageGene>();
        case ci::kLocalMaxGeneTypeId:
            return std::make_unique<ci::LocalMaxGene>();
        case ci::kLocalMinGeneTypeId:
            return std::make_unique<ci::LocalMinGene>();
        case ci::kLocalNormalizeGeneTypeId:
            return std::make_unique<ci::LocalNormalizeGene>();
        case ci::kLogGeneTypeId:
            return std::make_unique<ci::LogGene>();
        case ci::kMaxCGeneTypeId:
            return std::make_unique<ci::MaxCGene>();
        case ci::kMaxGeneTypeId:
            return std::make_unique<ci::MaxGene>();
        case ci::kMaxValueGeneTypeId:
            return std::make_unique<ci::MaxValueGene>();
        case ci::kMinCGeneTypeId:
            return std::make_unique<ci::MinCGene>();
        case ci::kMinGeneTypeId:
            return std::make_unique<ci::MinGene>();
        case ci::kMinValueGeneTypeId:
            return std::make_unique<ci::MinValueGene>();
        case ci::kMultiplyCGeneTypeId:
            return std::make_unique<ci::MultiplyCGene>();
        case ci::kMultiplyGeneTypeId:
            return std::make_unique<ci::MultiplyGene>();
        case ci::kNormalizeGeneTypeId:
            return std::make_unique<ci::NormalizeGene>();
        case ci::kRescaleGeneTypeId:
            return std::make_unique<ci::RescaleGene>();
        case ci::kResizeThenGaborGeneTypeId:
            return std::make_unique<ci::ResizeThenGaborGene>();
        case ci::kShiftGeneTypeId:
            return std::make_unique<ci::ShiftGene>();
        case ci::kSmoothBilateralGeneTypeId:
            return std::make_unique<ci::SmoothBilateralGene>();
        case ci::kSmoothMedianGeneTypeId:
            return std::make_unique<ci::SmoothMedianGene>();
        case ci::kSobelGeneTypeId:
            return std::make_unique<ci::SobelGene>();
        case ci::kSobelXGeneTypeId:
            return std::make_unique<ci::SobelXGene>();
        case ci::kSobelYGeneTypeId:
            return std::make_unique<ci::SobelYGene>();
        case ci::kSqrtGeneTypeId:
            return std::make_unique<ci::SqrtGene>();
        case ci::kSubtractCGeneTypeId:
            return std::make_unique<ci::SubtractCGene>();
        case ci::kSubtractGeneTypeId:
            return std::make_unique<ci::SubtractGene>();
        case ci::kThresholdGeneTypeId:
            return std::make_unique<ci::ThresholdGene>();
        case ci::kUnsharpenGeneTypeId:
            return std::make_unique<ci::UnsharpenGene>();
        default:
            break;
    }

    throw std::runtime_error(
        "Unrecognized gene id: '"
        + std::to_string(typeId)
        + "';");
}

int ci::ImageGeneFactory::getTypeId(const std::string& geneName) {
    const char* name = geneName.c_str();

    if (strncasecmp(name, ci::kAbsDifferenceGeneTypeName, 30) == 0) {
        return ci::kAbsDifferenceGeneTypeId;
    } else if (strncasecmp(name, ci::kAddCGeneTypeName, 30) == 0) {
        return ci::kAddCGeneTypeId;
    } else if (strncasecmp(name, ci::kAddGeneTypeName, 30) == 0) {
        return ci::kAddGeneTypeId;
    } else if (strncasecmp(name, ci::kAverageGeneTypeName, 30) == 0) {
        return ci::kAverageGeneTypeId;
    } else if (strncasecmp(name, ci::kAverageValueGeneTypeName, 30) == 0) {
        return ci::kAverageValueGeneTypeId;
    } else if (strncasecmp(name, ci::kCannyGeneTypeName, 30) == 0) {
        return ci::kCannyGeneTypeId;
    } else if (strncasecmp(name, ci::kConstantOutputGeneTypeName, 30) == 0) {
        return ci::kConstantOutputGeneTypeId;
    } else if (strncasecmp(name, ci::kDilateGeneTypeName, 30) == 0) {
        return ci::kDilateGeneTypeId;
    } else if (strncasecmp(name, ci::kErodeGeneTypeName, 30) == 0 ) {
        return ci::kErodeGeneTypeId;
    } else if (strncasecmp(name, ci::kExpGeneTypeName, 30) == 0) {
        return ci::kExpGeneTypeId;
    } else if (strncasecmp(name, ci::kGaborGeneTypeName, 30) == 0) {
        return ci::kGaborGeneTypeId;
    } else if (strncasecmp(name, ci::kGaussianBlurGeneTypeName, 30) == 0) {
        return ci::kGaussianBlurGeneTypeId;
    } else if (strncasecmp(name, ci::kLaplaceGeneTypeName, 30) == 0) {
        return ci::kLaplaceGeneTypeId;
    } else if (strncasecmp(name, ci::kLocalAverageGeneTypeName, 30) == 0) {
        return ci::kLocalAverageGeneTypeId;
    } else if (strncasecmp(name, ci::kLocalMaxGeneTypeName, 30) == 0) {
        return ci::kLocalMaxGeneTypeId;
    } else if (strncasecmp(name, ci::kLocalMinGeneTypeName, 30) == 0) {
        return ci::kLocalMinGeneTypeId;
    } else if (strncasecmp(name, ci::kLocalNormalizeGeneTypeName, 30) == 0) {
        return ci::kLocalNormalizeGeneTypeId;
    } else if (strncasecmp(name, ci::kLogGeneTypeName, 30) == 0) {
        return ci::kLogGeneTypeId;
    } else if (strncasecmp(name, ci::kMaxCGeneTypeName, 30) == 0) {
        return ci::kMaxCGeneTypeId;
    } else if (strncasecmp(name, ci::kMaxGeneTypeName, 30) == 0) {
        return ci::kMaxGeneTypeId;
    } else if (strncasecmp(name, ci::kMaxValueGeneTypeName, 30) == 0) {
        return ci::kMaxValueGeneTypeId;
    } else if (strncasecmp(name, ci::kMinCGeneTypeName, 30) == 0) {
        return ci::kMinCGeneTypeId;
    } else if (strncasecmp(name, ci::kMinGeneTypeName, 30) == 0) {
        return ci::kMinGeneTypeId;
    } else if (strncasecmp(name, ci::kMinValueGeneTypeName, 30) == 0) {
        return ci::kMinValueGeneTypeId;
    } else if (strncasecmp(name, ci::kMultiplyCGeneTypeName, 30) == 0) {
        return ci::kMultiplyCGeneTypeId;
    } else if (strncasecmp(name, ci::kMultiplyGeneTypeName, 30) == 0) {
        return ci::kMultiplyGeneTypeId;
    } else if (strncasecmp(name, ci::kNormalizeGeneTypeName, 30) == 0) {
        return ci::kNormalizeGeneTypeId;
    } else if (strncasecmp(name, ci::kRescaleGeneTypeName, 30) == 0) {
        return ci::kRescaleGeneTypeId;
    } else if (strncasecmp(name, ci::kResizeThenGaborGeneTypeName, 30) == 0) {
        return ci::kResizeThenGaborGeneTypeId;
    } else if (strncasecmp(name, ci::kShiftGeneTypeName, 30) == 0) {
        return ci::kShiftGeneTypeId;
    } else if (strncasecmp(name, ci::kSmoothBilateralGeneTypeName, 30) == 0) {
        return ci::kSmoothBilateralGeneTypeId;
    } else if (strncasecmp(name, ci::kSmoothMedianGeneTypeName, 30) == 0) {
        return kSmoothMedianGeneTypeId;
    } else if (strncasecmp(name, ci::kSobelGeneTypeName, 30) == 0) {
        return kSobelGeneTypeId;
    } else if (strncasecmp(name, ci::kSobelXGeneTypeName, 30) == 0) {
        return kSobelXGeneTypeId;
    } else if (strncasecmp(name, ci::kSobelYGeneTypeName, 30) == 0) {
        return kSobelYGeneTypeId;
    } else if (strncasecmp(name, ci::kSqrtGeneTypeName, 30) == 0) {
        return kSqrtGeneTypeId;
    } else if (strncasecmp(name, ci::kSubtractCGeneTypeName, 30) == 0) {
        return kSubtractCGeneTypeId;
    } else if (strncasecmp(name, ci::kSubtractGeneTypeName, 30) == 0) {
        return kSubtractGeneTypeId;
    } else if (strncasecmp(name, ci::kThresholdGeneTypeName, 30) == 0) {
        return kThresholdGeneTypeId;
    } else if (strncasecmp(name, ci::kUnsharpenGeneTypeName, 30) == 0) {
        return kUnsharpenGeneTypeId;
    }

    throw std::runtime_error(
        "Unrecognized gene name: '"
        + geneName
        + "';");
}
