set(PART1_BOXES_DECODER_SOURCES JpegPlenoSignatureBoxParser.cpp FileTypeBoxParser.cpp ../../../Common/Boxes/InMemoryDBoxContents.cpp)
add_library(jplm_part1_boxes_decoder ${PART1_BOXES_DECODER_SOURCES})
target_link_libraries(jplm_part1_boxes_decoder jplm_common_boxes_parsers)