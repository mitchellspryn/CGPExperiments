#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <unordered_map>

#include "DataChunk.hpp"
#include "RandomNumberGenerator.hpp"

namespace cgpExperiments {
namespace core {

// A class that can fill a datachunk with data.
// The current implementation copies data from a memory-mapped file.
class DataChunkProvider {
    public:
        DataChunkProvider(
            const std::unordered_map<std::string, std::string>& parameters,
            std::shared_ptr<RandomNumberGenerator> randomNumberGenerator);
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
        int sampleDataTypeSize_; // should be 4 or 1
        int fileSizeInBytes_;
        int sampleSizeInBytes_;

        std::shared_ptr<RandomNumberGenerator> randomNumberGenerator_;

        char* baseAddress_ = nullptr;

        void mapFileIntoMemory(const std::string& fileName);
        void releaseMappedFile(bool allowThrow);
        void fillParametersFromMap(
            const std::unordered_map<std::string, std::string>& parameters);
};

}
}
