set(PART1_DECODER_SOURCES  JPLFileFromStream.cpp
    JPLFileParser.cpp
    ../Common/JPLFile.cpp)

add_library(jplm_part1_decoder ${PART1_DECODER_SOURCES})