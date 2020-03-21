set(PART1_ENCODER_SOURCES  JPLFileFromStream.cpp
    JPLFileParser.cpp
    ../Common/JPLFile.cpp)

add_library(jplm_part1_encoder ${PART1_ENCODER_SOURCES})