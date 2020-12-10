#include <iostream>
#include <locale>
#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>

#include "../core/include/DataChunkProvider.hpp"
#include "../core/include/RandomNumberGenerator.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace cc = cgpExperiments::core;

static constexpr int gImageWidth = 500;
static constexpr int gImageHeight= 500;

std::shared_ptr<cc::RandomNumberGenerator> makeRandomNumberGenerator() {
    // Seed doesn't matter, there is no randomness.
    return std::make_shared<cc::RandomNumberGenerator>(42);
}

std::shared_ptr<cc::DataChunkProvider> makeProvider(
        const std::string& fileName,
        std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator) {
    std::unordered_map<std::string, std::string> params;
    params["sampleWidth"] = std::to_string(gImageWidth);
    params["sampleHeight"] = std::to_string(gImageHeight);
    params["fileName"] = fileName;

    std::shared_ptr<cc::DataChunkProvider> provider = 
        std::make_shared<cc::DataChunkProvider>(params, randomNumberGenerator);

    return provider;
}

void clearScreen() {
    int x = std::system("clear");
}

cv::Mat visualizeDataChunk(cc::DataChunk& chunk) {
    cv::Mat floatArray = cv::Mat(
        gImageHeight, 
        gImageWidth,
        CV_32FC1,
        chunk.getDataPtr());

    cv::Mat result;
    floatArray.convertTo(result, CV_8UC1);

    return result;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: ./displayImages <root_path>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string rootPath = std::string(argv[1]);

    std::cout << "Making providers..." << std::endl;

    std::shared_ptr<cc::RandomNumberGenerator> randomNumberGenerator = makeRandomNumberGenerator();

    std::shared_ptr<cc::DataChunkProvider> rProvider = makeProvider(rootPath + "_r.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> gProvider = makeProvider(rootPath + "_g.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> bProvider = makeProvider(rootPath + "_b.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> grayProvider = makeProvider(rootPath + "_gray.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> hProvider = makeProvider(rootPath + "_h.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> lProvider = makeProvider(rootPath + "_l.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> sProvider = makeProvider(rootPath + "_s.dat", randomNumberGenerator);
    std::shared_ptr<cc::DataChunkProvider> labelProvider = makeProvider(rootPath + "_label.dat", randomNumberGenerator);

    cc::DataChunk rDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk gDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk bDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk grayDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk hDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk lDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk sDataChunk(gImageWidth, gImageHeight, 1);
    cc::DataChunk labelDataChunk(gImageWidth, gImageHeight, 1);

    cv::namedWindow("Red", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Green", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Blue", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Gray", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Hue", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Lightness", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Saturation", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Color", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Label", CV_WINDOW_AUTOSIZE);

    bool shouldStop = false;
    int index = 0;
    int numSamplesInDataset = rProvider->getNumSamplesInDataset();
    while (!shouldStop) {
        rProvider->getRandomChunk(rDataChunk, index);
        gProvider->getRandomChunk(gDataChunk, index);
        bProvider->getRandomChunk(bDataChunk, index);
        grayProvider->getRandomChunk(grayDataChunk, index);
        hProvider->getRandomChunk(hDataChunk, index);
        lProvider->getRandomChunk(lDataChunk, index);
        sProvider->getRandomChunk(sDataChunk, index);
        labelProvider->getRandomChunk(labelDataChunk, index);

        cv::Mat rViz = visualizeDataChunk(rDataChunk);
        cv::Mat gViz = visualizeDataChunk(gDataChunk);
        cv::Mat bViz = visualizeDataChunk(bDataChunk);
        cv::Mat grayViz = visualizeDataChunk(grayDataChunk);
        cv::Mat hViz = visualizeDataChunk(hDataChunk);
        cv::Mat lViz = visualizeDataChunk(lDataChunk);
        cv::Mat sViz = visualizeDataChunk(sDataChunk);
        cv::Mat labelViz = visualizeDataChunk(labelDataChunk);

        cv::Mat rgb(gImageHeight, gImageWidth, CV_8UC3);
        for (int y = 0; y < gImageHeight; y++) {
            for (int x = 0; x < gImageWidth; x++) {
                cv::Point pt = cv::Point(x,y);
                cv::Vec3b out = rgb.at<cv::Vec3b>(pt);
                out[0] = bViz.at<uint8_t>(pt);
                out[1] = gViz.at<uint8_t>(pt);
                out[2] = rViz.at<uint8_t>(pt);
                rgb.at<cv::Vec3b>(pt) = out;
            }
        }

        cv::imshow("Red", rViz);
        cv::imshow("Green", gViz);
        cv::imshow("Blue", bViz);
        cv::imshow("Gray", grayViz);
        cv::imshow("Hue", hViz);
        cv::imshow("Lightness", lViz);
        cv::imshow("Saturation", sViz);
        cv::imshow("Color", rgb);
        cv::imshow("Label", labelViz);

        clearScreen();
        std::cout << "Displaying example " << index << " / " << numSamplesInDataset << "\n";
        std::cout << "Controls:\n";
        std::cout << "  N : Next image.\n";
        std::cout << "  B : previous image.\n";
        std::cout << "  Q : quit" << std::endl;

        int key = toupper(static_cast<char>(cv::waitKey(0)));
        if (key == 'N') {
            index++;
        } else if (key == 'B') {
            index--;
        } else if (key == 'Q') {
            shouldStop = true;
        }

        if (index < 0) {
            index = numSamplesInDataset - 1;
        } else if (index >= numSamplesInDataset) {
            index = 0;
        }
    }

    std::cout << "destroying all windows..." << std::endl;
    cv::destroyAllWindows();

    std::cout << "Graceful termination." << std::endl;
    return EXIT_SUCCESS;
}
