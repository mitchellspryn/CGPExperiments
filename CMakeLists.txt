cmake_minimum_required(VERSION 2.8.3)
#set(CMAKE_C_COMPILER "/usr/bin/clang")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(CgpExperiments)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -march=native -std=c++17 -O0 -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -march=native -std=c++17 -O3 -g -flto")

find_package(OpenMP REQUIRED)
find_package(OpenCV REQUIRED)

#SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")
#SET(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB core_SRC
    "core/src/*.cpp"
    "core/src/**/*.cpp"
)

file(GLOB fitCurve_SRC
    "fitCurve/src/*.cpp"
    "fitCurve/src/**/*.cpp"
)

file(GLOB image_SRC
    "image/src/*.cpp"
    "image/src/**/*.cpp"
)

include_directories(
    core/include
    fitCurve/include
    image/include
    ${OpenCV_INCLUDE_DIRS}
)

add_executable(TrainCgp
    application/trainCgp.cpp
    ${core_SRC}
    ${fitCurve_SRC}
    ${image_SRC}
)

target_link_libraries(TrainCgp
    ${OpenCV_LIBRARIES}
)

add_executable(GenerateCurve
    application/generateCurve.cpp
)

add_executable(TestRng
    application/testRng.cpp
    ${core_SRC}
)

add_executable(DumpBuffers
    application/dumpBuffers.cpp
    ${core_SRC}
    ${fitCurve_SRC}
    ${image_SRC}
)

target_link_libraries(DumpBuffers
    ${OpenCV_LIBRARIES}
)

add_executable(DisplayImages
    application/displayImages.cpp
    ${core_SRC}
)

target_link_libraries(DisplayImages
    ${OpenCV_LIBRARIES}
)

add_executable(GenePerfTest
    application/genePerfTest.cpp
    ${core_SRC}
    ${fitCurve_SRC}
    ${image_SRC}
)

target_link_libraries(GenePerfTest
    ${OpenCV_LIBRARIES}
)
