#include "../include/DataChunk.hpp"

namespace cc = cgpExperiments::core;

cc::DataChunk::DataChunk(int width, int height, int num) {
    width_ = width;
    height_ = height;
    num_ = num;
    num_ = maxNum_;
    size_ = width * height * num;

    data_.resize(size_, 0);
}

cc::DataChunk::setNum(int num) {
    num_ = num;
    size_ = width * height * num;

    if (num > maxNum_) {
        maxNum_ = num;
        capacity_ = size;
        data_.resize(size_, 0);
    }
}
