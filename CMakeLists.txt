cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(jplm)

set (JPLM_NAME "JPEG Pleno Model")

# The version number.
set (JPLM_VERSION_MAJOR 0)
set (JPLM_VERSION_MINOR 1)
set (JPLM_VERSION_PATCH 0)



#configuring paths
set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake;${CMAKE_MODULE_PATH}")
set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_DIR})


##puts the configuration file in the binary dir
configure_file (
  "${CMAKE_HOME_DIRECTORY}/cmake/JPLMConfig.h.in"
  "${PROJECT_BINARY_DIR}/JPLMConfig.h"
  )


include_directories("${PROJECT_BINARY_DIR}")


include_directories(.)
include_directories(cfg)
include_directories(doc)
include_directories(source)

#configuring the compiler
include(JPLMWarnings)
include(JPLMCompilerOptimizations)

#including external libraries by downloading them from git..
include(ExternalGoogleTest)
include(ExternalCLI11)
include(ExternalCPPItertools)
include(ExternalNlohmannJSON)
include(ExternalOpenJPEG)

link_libraries(stdc++fs)

# directories here header files may be found
# include_directories(.)
# include_directories(cfg)
# include_directories(doc)
include_directories(source)

#setting up the doxygen documentation build
option(BUILD_DOC "Build documentation" ON)
# option(BUILD_DOC "Build documentation" OFF)


add_executable(jpl-encoder-bin
        source/App/Encoder/jpl-encoder.cpp
        CMakeLists.txt)

target_link_libraries(jpl-encoder-bin PRIVATE jplm_part1_common)
target_link_libraries(jpl-encoder-bin PRIVATE jplm_part2_encoder)
target_link_libraries(jpl-encoder-bin PRIVATE jplm_part2_common) 
target_link_libraries(jpl-encoder-bin PRIVATE image) 

add_executable(jpl-decoder-bin
        source/App/Decoder/jpl-decoder.cpp
        CMakeLists.txt)

target_link_libraries(jpl-decoder-bin PRIVATE jplm_part1_common)
target_link_libraries(jpl-decoder-bin PRIVATE jplm_part2_encoder)
target_link_libraries(jpl-decoder-bin PRIVATE jplm_part2_common) 
target_link_libraries(jpl-decoder-bin PRIVATE image) 

add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/App/Utils/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Utils/Image/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Utils/Stream/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Common/Boxes/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Common/Boxes/Generic/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Common/Boxes/Parsers)

# add_library(jplm_boxes_decoding)
# target_link_libraries(jplm_boxes_decoding jplm_common_boxes_parsers jplm_common_boxes)


add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part1/Common/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part1/Common/Boxes)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part1/Decoder/Boxes)

# target_link_libraries(jplm_common_boxes_parsers jplm_part1_boxes_decoder)

add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Common/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Common/Boxes)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Decoder/Boxes)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Decoder/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Encoder/)



enable_testing()
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Utils/Image/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Utils/Stream/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/ThirdParty/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part2/Common/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Common/Boxes/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Common/Boxes/Generic/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Common/Boxes/Parsers/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part1/Common/Boxes/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part1/Common/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part1/Encoder/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part1/Decoder/Boxes/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part1/Decoder/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part2/Common/Boxes)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part2/Decoder/Boxes)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part2/Decoder/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Common/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part2/Encoder/)


if(BUILD_DOC)
  file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/doc/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Documentation build started")
    add_custom_target( doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen must be installed to generate the documentation.")
  endif (DOXYGEN_FOUND)
endif(BUILD_DOC)