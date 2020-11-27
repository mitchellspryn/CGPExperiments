cmake_minimum_required(VERSION 2.8.3)
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
project(CgpExperiments)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -march=native -std=c++17")

find_package(OpenMP REQUIRED)

SET(CMAKE_BUILD_TYPE "Debug")
#SET(CMAKE_BUILD_TYPE "Release")
SET(CMAKE_VERBOSE_MAKEFILE ON)

file(GLOB core_SRC
    "core/src/*.cpp"
    "core/src/**/*.cpp"
)

file(GLOB fitCurve_SRC
    "fitCurve/src/*.cpp"
    "fitCurve/src/**/*.cpp"
)

include_directories(
    core/include
    fitCurve/include
)

message("******************")
message(${core_SRC})
message("??????????????????")

message("******************")
message(${fitCurve_SRC})
message("??????????????????")

add_executable(TrainCgp
    application/trainCgp.cpp
    ${core_SRC}
    ${fitCurve_SRC}
)

