#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

#include "DataChunk.hpp"
#include "RandomNumberGenerator.hpp"

namespace cgpExperiments {
namespace core {

class DataChunkProvider {
    public:
        DataChunkProvider(
            const std::unordered_map<std::string, std::string>& dataChunkParameters);
        ~DataChunkProvider();

        inline int getChunkWidth() {
            return sampleWidth_;
        }

        inline int getChunkHeight() {
            return sampleHeight_;
        }

        inline int getNumSamplesInDataset() {
            return numSamples_;
        }

        void getRandomChunk(DataChunk& chunk, int startIndex = -1);
    
    private:
        std::string fileName_;
        int fileDescriptor_;

        int sampleWidth_;
        int sampleHeight_;
        int numSamples_;
        int fileSizeInBytes_;
        int sampleSizeInBytes_;

        float* baseAddress_ = nullptr;

        void mapFileIntoMemory(const std::string& fileName);
        void releaseMappedFile(bool allowThrow);
};

}
}
