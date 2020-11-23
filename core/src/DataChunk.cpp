#include "../include/DataChunk.hpp"

namespace cc = cgpExperiments::core;

cc::DataChunk::DataChunk(int width, int height, int num) {
    width_ = width;
    height_ = height;
    num_ = num;
    size_ = width * height * num;

    data_.resize(size_, 0);
}
