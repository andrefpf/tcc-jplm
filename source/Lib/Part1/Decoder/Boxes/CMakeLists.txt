set(PART1_BOXES_DECODER_SOURCES
    JpegPlenoSignatureBoxParser.cpp
    FileTypeBoxParser.cpp
    ../../Common/Boxes/CommonExceptions.cpp
    ../../Common/Boxes/FileTypeBox.cpp
    ../../Common/Boxes/FileTypeContents.cpp
    ../../Common/Boxes/JpegPlenoCodestreamBox.cpp
    ../../Common/Boxes/JpegPlenoFileTypeContents.cpp
    ../../Common/Boxes/JpegPlenoSignatureBox.cpp
    ../../Common/Boxes/JpegPlenoSignatureContents.cpp
    ../../Common/Boxes/JpegPlenoThumbnailBox.cpp
    ../../Common/Boxes/JpegPlenoThumbnailContents.cpp)

add_library(jplm_part1_boxes_decoder ${PART1_BOXES_DECODER_SOURCES})
target_link_libraries(jplm_part1_boxes_decoder stream)
target_link_libraries(jplm_part1_boxes_decoder jplm_common_boxes_parsers)