set(COMMON_BOXES_PARSERS_SOURCES BoxParserRegistry.cpp
    ColourSpecificationBoxParser.cpp
    ContiguousCodestreamBoxParser.cpp
    BoxParserRegistry.cpp)

add_library(jplm_common_boxes_parsers ${COMMON_BOXES_PARSERS_SOURCES})

target_link_libraries(jplm_common_boxes_parsers
    jplm_common_boxes_generic
    jplm_common_boxes
    jplm_part2_common
    jplm_part2_boxes_decoder
    jplm_part1_boxes_decoder
    jplm_part2_common_boxes
    jplm_part1_decoder)