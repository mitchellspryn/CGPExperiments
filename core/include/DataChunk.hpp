#pragma once

#include <memory>
#include <vector>

namespace cgpExperiments {
namespace core {

// A class representing a chunk of data that can be an input/output to a gene.
class DataChunk {
    public:
        DataChunk(int width, int height, int num, int dataTypeSize);

        inline int getWidth() const { return width_; }
        inline int getHeight() const { return height_; }
        inline int getNum() const { return num_; }
        inline int getDataTypeSize() const { return dataTypeSize_; }
        inline int getSizeInSamples() const { return sizeInSamples_; }
        inline int getSizeInBytes() const { return sizeInBytes_; }
        inline int getCapacityInSamples() const { return capacityInSamples_; }
        inline int getCapacityInBytes() const { return capacityInBytes_; }

        inline float* getFloatDataPtr() { 
            return reinterpret_cast<float*>(data_.data());
        }

        inline const float* getConstFloatDataPtr() const { 
            return reinterpret_cast<const float*>(data_.data());
        }

        inline unsigned char* getCharDataPtr() {
            return data_.data();
        }

        inline const unsigned char* getConstCharDataPtr() const {
            return const_cast<const unsigned char*>(data_.data());
        }

        inline void setNum(int num);

    private:
        int width_;
        int height_;
        int num_;
        int dataTypeSize_;
        int maxNum_;
        int sizeInSamples_;
        int sizeInBytes_;
        int capacityInSamples_;
        int capacityInBytes_;

        std::vector<unsigned char> data_; 
};

}
}
