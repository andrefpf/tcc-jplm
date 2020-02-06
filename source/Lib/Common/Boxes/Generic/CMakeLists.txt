set(COMMON_BOXES_GENERIC_SOURCES
    ContiguousCodestreamCode.cpp
    ContiguousCodestreamCodeInMemory.cpp
    ContiguousCodestreamBox.cpp
    ContiguousCodestreamContents.cpp
    ImageHeaderContents.cpp
    UniversalUniqueIdentifier.cpp
    UUIDContents.cpp)
add_library(jplm_common_boxes_generic ${COMMON_BOXES_GENERIC_SOURCES})