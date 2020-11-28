#pragma once

#include <memory>
#include <vector>

namespace cgpExperiments {
namespace core {

class DataChunk {
    public:
        DataChunk(int width, int height, int num);

        inline int getWidth() const { return width_; }
        inline int getHeight() const { return height_; }
        inline int getNum() const { return num_; }
        inline int getSize() const { return size_; }
        inline int getCapacity() const { return capacity_; }
        inline float* getDataPtr() { return data_.data(); }
        inline const float* getConstDataPtr() const { return data_.data(); }
        inline void setNum(int num);

    private:
        int width_;
        int height_;
        int num_;
        int maxNum_;
        int size_;
        int capacity_;

        int widthHeight_;
        std::vector<float> data_; 
};

}
}
