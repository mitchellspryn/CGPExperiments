#include "../include/DataChunk.hpp"

namespace cc = cgpExperiments::core;

cc::DataChunk::DataChunk(int width, int height, int num, int dataTypeSize) {
    width_ = width;
    height_ = height;
    num_ = num;
    dataTypeSize_ = dataTypeSize;
    maxNum_ = num;
    sizeInSamples_ = width * height * num;
    sizeInBytes_ = sizeInSamples_ * dataTypeSize_;
    capacityInSamples_ = sizeInSamples_;
    capacityInBytes_ = sizeInBytes_;
    data_.resize(sizeInBytes_, 0);
}

void cc::DataChunk::setNum(int num) {
    num_ = num;
    sizeInSamples_ = width_ * height_ * num;
    sizeInBytes_ = sizeInSamples_ * dataTypeSize_;

    if (num > maxNum_) {
        maxNum_ = num;
        capacityInSamples_ = sizeInSamples_;
        capacityInBytes_ = sizeInBytes_;
        data_.resize(sizeInBytes_, 0);
    }
}
