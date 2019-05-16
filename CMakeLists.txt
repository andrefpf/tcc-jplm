set(CMAKE_CXX_STANDARD 17)

include_directories(.)
include_directories(cfg)
include_directories(doc)
include_directories(source)

cmake_minimum_required(VERSION 3.14)


## GTEST
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in.gtest googletest-download/CMakeLists.txt)
#
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
## the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

## The gtest/gtest_main targets carry header search path
## dependencies automatically when using CMake 2.8.11 or
## later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

## OPENJPEG

# Download and unpack openjpeg at configure time
#configure_file(CMakeLists.txt.in.openjpeg openjpeg-download/CMakeLists.txt)

include(CMakeLists.txt.in.openjpeg)
include_directories("${OPJ_INCLUDE}")


#configure_file(CMakeLists.txt.in.openjpeg openjpeg-download/CMakeLists.txt)
#
#execute_process(COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release -G "${CMAKE_GENERATOR}" .
#        RESULT_VARIABLE result
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/openjpeg-download)
#if(result)
#  message(FATAL_ERROR "CMake step for openjpeg failed: ${result}")
#endif()
#
#execute_process(COMMAND ${CMAKE_COMMAND} --build .
#        RESULT_VARIABLE result
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/openjpeg-download)
#if(result)
#  message(FATAL_ERROR "Build step for openjpeg failed: ${result}")
#endif()
#
#execute_process(COMMAND make
#        RESULT_VARIABLE result
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/openjpeg-build)
#if(result)
#  message(FATAL_ERROR "Build step for openjpeg failed: ${result}")
#endif()

#
#add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/openjpeg-src
#        ${CMAKE_CURRENT_BINARY_DIR}/openjpeg-build
#        EXCLUDE_FROM_ALL)

#include_directories(${OPENJPEG_BINARY_DIR}/src/lib)
#include_directories(${OPENJPEG_SOURCE_DIR}/src/lib)



## EXECUTABLES
project(jplm)

add_executable(jpl-encoder-bin
        source/App/Encoder/jpl-encoder.cpp
        CMakeLists.txt)

add_executable(jpl-decoder-bin
        source/App/Decoder/jpl-decoder.cpp
        CMakeLists.txt)

add_executable(example source/Tests/example_test.cpp)

## LINKING
target_link_libraries(example gtest_main ${OPJ_LIB})



## TESTS
#add_test(NAME simple_test COMMAND example)