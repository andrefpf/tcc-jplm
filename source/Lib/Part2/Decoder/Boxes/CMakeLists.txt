set(PART2_BOXES_DECODER_SOURCES
    CameraParameterBoxParser.cpp
    ProfileAndLevelBoxParser.cpp
    LightFieldHeaderBoxParser.cpp
    JpegPlenoLightFieldHeaderBoxParser.cpp
    JpegPlenoLightFieldBoxParser.cpp
    ../../../Common/Boxes/InMemoryDBox.cpp)

add_library(jplm_part2_boxes_decoder ${PART2_BOXES_DECODER_SOURCES})

target_link_libraries(jplm_part2_boxes_decoder jplm_common_boxes_parsers)