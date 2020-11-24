#pragma once

#include <memory>

namespace cgpExperiments {
namespace core {

class DataChunk {
    public:
        DataChunk(int width, int height, int num);

        inline int getWidth() { return width_; }
        inline int getHeight() { return height_; }
        inline int getNum() { return num_; }
        inline int getSize() { return size_; }
        inline int getCapacity() { return capacity_; }
        float* getDataPtr() { return data_.data(); }
        inline void setNum(int num);

    private:
        int width_;
        int height_;
        int num_;
        int maxNum_;
        int size_;
        int capacity_;
        std::vector<float> data_; 
};

}
}
