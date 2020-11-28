#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

float computeFunction(float x) {
    return (2.0 * std::sin(3.0 * x)) - (0.4 * x) + 1;
}

int main(int argc, char** argv) {
    std::cout << "Generating datasets..." << std::endl;

    float minValue = -10.0f;
    float maxValue = 10.0f;
    float step = 0.1;
    int numSteps = static_cast<int>(std::ceil((maxValue - minValue) / step));

    const std::string xFileName = "x.dat";
    const std::string yFileName = "y.dat";
    std::ofstream xStream(xFileName, std::ios::out | std::ios::binary);
    std::ofstream yStream(yFileName, std::ios::out | std::ios::binary);

    if (!xStream.good()) {
        throw std::runtime_error(
            "Could not open file '"
            + xFileName
            + "' for writing.");
    }

    if (!yStream.good()) {
        throw std::runtime_error(
            "Could not open file '"
            + yFileName
            + "' for writing.");
    }

    int numSamples = 0;
    for (int i = 0; i < numSteps; i++) {
        float xVal = minValue + (static_cast<float>(i) * step);
        float yVal = computeFunction(xVal);

        std::cout << 
            std::to_string(xVal)
            << " -> "
            << std::to_string(yVal)
            << "\n";

        xStream.write(
            reinterpret_cast<char*>(&xVal), sizeof(float));
        yStream.write(
            reinterpret_cast<char*>(&yVal), sizeof(float));

        numSamples++;
    }
    
    std::cout << "Generated datasets.\n";
    std::cout << "dimensions are <width, height, num> = <1, 1, " << std::to_string(numSamples) << ">" << std::endl;
    return EXIT_SUCCESS;
}
