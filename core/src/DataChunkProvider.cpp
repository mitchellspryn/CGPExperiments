#include "../include/DataChunkProvider.hpp"

#include <filesystem>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

namespace cc = cgpExperiments::core;

cc::DataChunkProvider::DataChunkProvider(const std::string& fileName, int sampleWidth, int sampleHeight) {
    sampleWidth_ = sampleWidth;
    sampleHeight_ = sampleHeight;
    fileSizeInBytes_ = std::filesystem::file_size(fileName);
    fileName_ = fileName;

    sampleSizeInBytes_ = sampleWidth_ * sampleHeight_ * sizeof(float);
    if ((fileSizeInBytes_ % sampleSizeInBytes_) != 0) {
        throw std::runtime_error(
                "Provided input file '" 
                + fileName 
                + "' has a size of " 
                + fileSizeInBytes_ 
                + " bytes, which is not divisible by " 
                + std::to_string(divisor) 
                + " as it should be.");
    }

    numSamples_ = fileSizeInBytes_ / sampleSizeInBytes_;
    
    mapFileIntoMemory(fileName_);
}

cc::DataChunkProvider::~DataChunkProvider() {
    releaseMappedFile();
}

int cc::DataChunkProvider::getNumSamplesInDataset() {
    return numSamples_;
};

void cc::DataChunkProvider::getRandomChunk(DataChunk& chunk, RandomNumberGenerator& rng, int startIndex = -1) {
    if (startIndex >= numSamples_) {
        throw std::runtime_error(
                "Attempted to grab data at (zero-based) index "
                + std::to_string(startIndex)
                + ", but there are only "
                + std::to_string(numSamples_)
                + " samples in the dataset.");
    }
    else if (startIndex < 0) {
        float randomFloat = rng.getRandomFloat(); 
        startIndex = static_cast<int>(std::floor(numSamples_*randomFloat)) % numSamples_;
    }

    // If we select a portion of the data near the end of the file, 
    //     we may need to wrap around
    int firstCopyNumSamples = std::min(chunk.getNum(), numSamples_ - startIndex);
    int numBytesToCopy = firstCopyNumSamples * sampleSizeInBytes_;

    float* chunkBuffer = chunk.getDataPtr();
    float* data = baseAddress_ + (sampleSizeInBytes_ * startIndex);

    std::memcpy(chunkBuffer, data, numBytesToCopy);

    int secondCopyNumSamples = chunk.getNum() - firstCopyNumSamples;
    if (secondCopyNumSamples == 0) {
        return;
    }

    chunkBuffer += numBytesToCopy;
    data = baseAddress_;
    numBytesToCopy = secondCopyNumSamples * sampleSizeInBytes_;
    std::memcpy(chunkBuffer, data, numBytesToCopy);
}

void cc::DataChunkProvider::mapFileIntoMemory(const std::string& fileName) {
    fileDescriptor_ = open(fileName, O_RDONLY);
    if (fileDescriptor_ < 0) {
        throw std::runtime_error(
                "Cannot open '" 
                + fileName
                + "' for reading.");
    }

    void* mappedMemory = mmap(
            NULL, // let kernel decide mapping address
            fileSizeInBytes_,
            PROT_READ,
            MAP_SHARED_VALIDATE,
            fileDescriptor_,
            0); // offset

    int error = errno;

    if (mappedMemory == (void*)-1) {
        close(fileDescriptor_);
        throw std::runtime_error(
                "Cannot map '"
                + fileName
                + "' into memory. Mmap returns '"
                + std::stoi(error)
                + "', which is error code '"
                + std::string(strerror(error))
                + "'.");
    }

    baseAddress_ = reinterpret_cast<float*>(mappedMemory);
}

void cc::DataChunkProvider::releaseMappedFile(bool allowThrow) {
    int ret = 0;
    int error = 0;

    std::string errorMessage = "";
    if (baseAddress_) {
        ret = munmap(reinterpret_cast<float*>(baseAddress__, fileSizeInBytes_));

        if ((ret != 0) && allowThrow) {
            errorMessage += 
                    "Cannot unmap '"
                    + fileName_
                    + "' from memory. Munmap returns '"
                    + std::stoi(error)
                    + "', which is error code '"
                    + std::string(strerror(error))
                    + "'.";
        }

        baseAddress_ = nullptr;
    }

    if (fileDescriptor_) {
        ret = close(fileDescriptor_);
        error = errno;
        
        if (ret != 0) {
            errorMessage += 
                    "Cannot close file descriptor for '"
                    + fileName_
                    + "'. Close returns '"
                    + std::stoi(error)
                    + "', which is error code '"
                    + std::string(strerror(error))
                    + "'.";
        }
    }

    // This can be called from dtor, which cannot safely throw exceptions.
    // If that is the case, it's likely the program is getting torn down, 
    //     so the resources will be freed shortly anyways.
    if (allowThrow && (errorMessage.size() > 0)) {
        throw std::runtime_error(errorMessage);
    }
}
