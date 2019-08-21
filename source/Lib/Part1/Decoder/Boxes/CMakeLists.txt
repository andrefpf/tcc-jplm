set(PART1_DECODER_SOURCES JpegPlenoSignatureBoxParser.cpp FileTypeBoxParser.cpp ../../../Common/Boxes/InMemoryDBoxContents.cpp)
add_library(jplm_part1_decoder ${PART1_DECODER_SOURCES})