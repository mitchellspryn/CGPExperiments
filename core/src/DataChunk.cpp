#include "../include/DataChunk.hpp"

namespace cc = cgpExperiments::core;

cc::DataChunk::DataChunk(int width, int height, int num) {
    width_ = width;
    height_ = height;
    num_ = num;
    maxNum_ = num;
    size_ = width * height * num;
    widthHeight_ = width * height;

    data_.resize(size_, 0);
}

void cc::DataChunk::setNum(int num) {
    num_ = num;
    size_ = width_ * height_ * num;

    if (num > maxNum_) {
        maxNum_ = num;
        capacity_ = size_;
        data_.resize(size_, 0);
    }
}
