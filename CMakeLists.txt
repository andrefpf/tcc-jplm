cmake_minimum_required(VERSION 3.10)

project(jplm)

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake;${CMAKE_MODULE_PATH}")
# find_package(OpenJPEG)

set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#including external libraries by downloading them from git..
include(ExternalOpenJPEG)

include(ExternalGoogleTest)

add_executable(example source/Tests/example_test.cpp)

add_dependencies(example openjpeg)
target_link_libraries(example ${openjpeg_LIB})



target_link_libraries(example gtest_main)


include_directories(.)
include_directories(cfg)
include_directories(doc)
include_directories(source)



# set(openjpeg_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/openjpeg-src/src/lib/openjp2/")
# include_directories(${openjpeg_INCLUDE_DIR})
# message("open jpeg include dir: ${openjpeg_INCLUDE_DIR}")

add_executable(jpl-encoder-bin
        source/App/Encoder/jpl-encoder.cpp
        CMakeLists.txt)

add_executable(jpl-decoder-bin
        source/App/Decoder/jpl-decoder.cpp
        CMakeLists.txt)


# add_dependencies(example openjpeg)

