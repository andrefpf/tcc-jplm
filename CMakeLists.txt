cmake_minimum_required(VERSION 3.14)
project(jplm)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(cfg)
include_directories(doc)
include_directories(source)

add_executable(jpl-encoder-bin
        source/App/Encoder/jpl-encoder.cpp
        CMakeLists.txt)

add_executable(jpl-decoder-bin
        source/App/Decoder/jpl-decoder.cpp
        CMakeLists.txt)