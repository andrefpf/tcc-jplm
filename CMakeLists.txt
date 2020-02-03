cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(jplm)

set(JPLM_NAME "JPEG Pleno Model")

# The version number.
set(JPLM_VERSION_MAJOR 0)
set(JPLM_VERSION_MINOR 1)
set(JPLM_VERSION_PATCH 0)

set(CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake;${CMAKE_MODULE_PATH}")


#configuring paths
if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib-debug)
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin-debug)
else (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_LIBRARY_DIR ${CMAKE_SOURCE_DIR}/lib)
  set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
endif (CMAKE_BUILD_TYPE MATCHES Debug)


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_DIR})

include(JPLMCompilerOptimizations)

##puts the configuration file in the binary dir
configure_file(
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


#including external libraries by downloading them from git..
include(ExternalGoogleTest)
include(ExternalMagic_Enum)
include(ExternalCLI11)
include(ExternalCppConsoleTable)
include(ExternalCXXProperties)
include(ExternalCPPItertools)
include(ExternalNlohmannJSON)
include(ExternalOpenJPEG)

if (UNIX)
    option(VISUALIZATION_TOOL "Compiles the Light field Visualization tool" OFF)
    FIND_PACKAGE(X11)
    if (VISUALIZATION_TOOL)
      if (X11_FOUND)
        message(STATUS "\nFOUND X11-LIBS")
      else (X11_FOUND)
        message(STATUS "\nX11-LIBS NOT FOUND")
        message(STATUS "\nYou may use the cmake flag -DVISUALIZATION_TOOL=OFF to
                skip the lightfield visualization tool compilation
                (e.g ~/jplm/build$ cmake -DVISUALIZATION_TOOL=OFF ..).")
        message(FATAL_ERROR "X11 is required for the Light field visualization tool.")
      endif (X11_FOUND)
    else (VISUALIZATION_TOOL)
      message(STATUS "\nLIGHTFIELD VISUALIZATION TOOL TARGET IS DISABLED")
    endif (VISUALIZATION_TOOL)
endif (UNIX)

if(CMAKE_COMPILER_IS_GNUCXX)
    link_libraries(stdc++fs)
endif()

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
    source/Lib/Common/JPLMConfiguration.cpp
    CMakeLists.txt)

target_link_libraries(jpl-encoder-bin PRIVATE jplm_part1_common)
target_link_libraries(jpl-encoder-bin PRIVATE jplm_part2_encoder)
target_link_libraries(jpl-encoder-bin PRIVATE jplm_part2_common)
target_link_libraries(jpl-encoder-bin PRIVATE stream)
target_link_libraries(jpl-encoder-bin PRIVATE image)
target_link_libraries(jpl-encoder-bin PRIVATE jplm_part2_encoder_transform_mode)

add_executable(jpl-decoder-bin
    source/App/Decoder/jpl-decoder.cpp
    source/Lib/Common/JPLMConfiguration.cpp
    CMakeLists.txt)

target_link_libraries(jpl-decoder-bin PRIVATE jplm_part2_common)
target_link_libraries(jpl-decoder-bin PRIVATE jplm_part2_decoder)
target_link_libraries(jpl-decoder-bin PRIVATE jplm_part2_boxes_decoder)
target_link_libraries(jpl-decoder-bin PRIVATE jplm_common_boxes_parsers)
target_link_libraries(jpl-decoder-bin PRIVATE jplm_part2_decoder_transform_mode)
target_link_libraries(jpl-decoder-bin PRIVATE stream)


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
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Common/TransformMode)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Decoder/Boxes)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Decoder/TransformMode)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Decoder/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Encoder/)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Lib/Part2/Encoder/TransformMode)


enable_testing()
include(JPLMTests)

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
add_subdirectory(${CMAKE_HOME_DIRECTORY}/source/Tests/Part2/Common/TransformMode/)


if (BUILD_DOC)
  file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_SOURCE_DIR}/doc/Doxyfile)
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Documentation build started")
    add_custom_target(doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)
  else (DOXYGEN_FOUND)
    message("Doxygen must be installed to generate the documentation.")
  endif (DOXYGEN_FOUND)
endif (BUILD_DOC)
