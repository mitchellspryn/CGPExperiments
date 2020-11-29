#include "../include/DataChunkProvider.hpp"

#include <cstring>
#include <fcntl.h>
#include <filesystem>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

#include <omp.h>

namespace cc = cgpExperiments::core;

cc::DataChunkProvider::DataChunkProvider(
        const std::unordered_map<std::string, std::string>& parameters,
        std::shared_ptr<RandomNumberGenerator> randomNumberGenerator) {
    randomNumberGenerator_ = randomNumberGenerator;
    
    fillParametersFromMap(parameters);
    fileSizeInBytes_ = std::filesystem::file_size(fileName_);

    sampleSizeInBytes_ = sampleWidth_ * sampleHeight_ * sizeof(float);
    if ((fileSizeInBytes_ % sampleSizeInBytes_) != 0) {
        throw std::runtime_error(
                "Provided input file '" 
                + fileName_ 
                + "' has a size of " 
                + std::to_string(fileSizeInBytes_)
                + " bytes, which is not divisible by " 
                + std::to_string(sampleSizeInBytes_) 
                + " as it should be.");
    }

    numSamples_ = fileSizeInBytes_ / sampleSizeInBytes_;
    
    mapFileIntoMemory(fileName_);
}

cc::DataChunkProvider::~DataChunkProvider() {
    releaseMappedFile(false);
}

void cc::DataChunkProvider::getRandomChunk(DataChunk& chunk, int startIndex) {
    if (startIndex >= numSamples_) {
        throw std::runtime_error(
                "Attempted to grab data at (zero-based) index "
                + std::to_string(startIndex)
                + ", but there are only "
                + std::to_string(numSamples_)
                + " samples in the dataset.");
    }
    else if (startIndex < 0) {
        startIndex = randomNumberGenerator_->getRandomInt(0, numSamples_ - 1);
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
    fileDescriptor_ = open(fileName.c_str(), O_RDONLY);
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
            // TODO: should use MAP_SHARED_VALIDATE, but apparently my glibc is too old
            //MAP_SHARED_VALIDATE,
            MAP_SHARED,
            fileDescriptor_,
            0); // offset

    int error = errno;

    if (mappedMemory == (void*)-1) {
        close(fileDescriptor_);
        throw std::runtime_error(
                "Cannot map '"
                + fileName
                + "' into memory. Mmap returns '"
                + std::to_string(error)
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
        ret = munmap(static_cast<void*>(baseAddress_), fileSizeInBytes_);

        if (ret != 0) {
            errorMessage += 
                    "Cannot unmap '"
                    + fileName_
                    + "' from memory. Munmap returns '"
                    + std::to_string(error)
                    + "', which is error code '"
                    + std::string(strerror(error))
                    + "'.\n";
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
                    + std::to_string(error)
                    + "', which is error code '"
                    + std::string(strerror(error))
                    + "'.\n";
        }
    }

    if (errorMessage.size() > 0) {
        // This function can be called from dtor, which cannot safely throw exceptions.
        // If that is the case, it's likely the program is getting torn down, 
        //     so the resources will be freed shortly anyways.
        if (allowThrow) {
            throw std::runtime_error(errorMessage);
        } else {
            std::cerr << errorMessage << std::endl;
        }
    }
}

void cc::DataChunkProvider::fillParametersFromMap(
        const std::unordered_map<std::string, std::string>& parameters) {
    sampleWidth_ = std::stoi(parameters.at("sampleWidth"));
    sampleHeight_ = std::stoi(parameters.at("sampleHeight"));
    fileName_ = parameters.at("fileName");
}
