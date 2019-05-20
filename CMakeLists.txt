set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(cfg)
include_directories(doc)
include_directories(source)

cmake_minimum_required(VERSION 3.10)

project(jplm)

#configuring paths
set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake;${CMAKE_MODULE_PATH}")
set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#configuring the compiler
set(CMAKE_CXX_STANDARD 17)
include(JPLMWarnings)

#including external libraries by downloading them from git..
include(ExternalOpenJPEG)
include(ExternalGoogleTest)

# directories here header files may be found
include_directories(.)
include_directories(cfg)
include_directories(doc)
include_directories(source)
include_directories(source/Lib/Utils/Image)

#defines an example executable, used to test external libraries...
add_executable(example source/Tests/example_test.cpp)
#defines a dependencie between example executable and openjpeg libray
add_dependencies(example openjpeg) #this is necessary in this case because 
#otherwise the executable may not find the include.

target_include_directories(example PRIVATE "${INCLUDE_DIRECTORIES};${openjpeg_INCLUDE}")

#informs the linker that it needs a library
# target_link_libraries(example ${openjpeg_LIB})
target_link_libraries(example openjp2)
target_link_libraries(example gtest_main)


add_executable(jpl-encoder-bin
        source/App/Encoder/jpl-encoder.cpp
        CMakeLists.txt)

add_executable(jpl-decoder-bin
        source/App/Decoder/jpl-decoder.cpp
        CMakeLists.txt)

add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Utils/Image/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Utils/Image/)