#pragma once

#include <atomic>
#include <string>

#include "DataChunk.hpp"
#include "RandomNumberGenerator.hpp"

namespace cgpExperiments {
namespace core {

class DataChunkProvider {
    public:
        DataChunkProvider(const std::string& fileName, int sampleWidth, int sampleHeight);
        ~DataChunkProvider();

        int getNumSamplesInDataset();
        void getRandomChunk(DataChunk& chunk, RandomNumberGenerator& rng, int startIndex = -1);
    
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
        void releaseMappedFile();
};

}
}
