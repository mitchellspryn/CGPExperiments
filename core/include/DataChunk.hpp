#pragma once

#include <memory>

namespace cgpExperiments {
namespace core {

class DataChunk {
    public:
        int width;
        int height;
        int num;
        float* data; 
};

}
}
