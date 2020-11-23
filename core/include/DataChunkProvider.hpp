#pragma once

#include <atomic>
#include <string>

#include "DataChunk.hpp"

namespace cgpExperiments {
namespace core {

class DataChunkProvider {
    public:
        DataChunkProvider(const std::string& fileName, int sampleWidth, int sampleHeight);
        ~DataChunkProvider();

        int getNumChunksInDataset();
        void getRandomChunk(DataChunk& chunk, int startIndex = -1);
    
    private:
        std::atomic<bool> fileMapped;

        mapFileIntoMemory(const std::string& fileName);
        releaseMappedFile();
};

}
}
