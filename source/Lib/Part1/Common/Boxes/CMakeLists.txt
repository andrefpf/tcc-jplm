set(PART1_COMMON_BOXES_SOURCES
    CommonExceptions.cpp
    FileTypeBox.cpp
    FileTypeContents.cpp
    JpegPlenoCodestreamBox.cpp
    JpegPlenoFileTypeContents.cpp
    JpegPlenoSignatureBox.cpp
    JpegPlenoSignatureContents.cpp
    JpegPlenoThumbnailBox.cpp
    JpegPlenoThumbnailContents.cpp
    ../../../Common/Boxes/Box.cpp
    ../../../Common/Boxes/InFileDBox.cpp
    ../../../Common/Boxes/InMemoryDBox.cpp)

add_library(jplm_part1_common_boxes ${PART1_COMMON_BOXES_SOURCES})