set(COMMON_BOXES_GENERIC_SOURCES
    BitsPerComponentBox.cpp
    ChannelDefinitionBox.cpp
    ChannelDefinitionContents.cpp
    ChannelDescription.cpp
    CharArrayContents.cpp
    ColourSpecificationBox.cpp
    ColourSpecificationContents.cpp
    CompressionType.cpp
    CompressionTypeImage.cpp
    ContiguousCodestreamBox.cpp
    ContiguousCodestreamCode.cpp
    ContiguousCodestreamCodeInMemory.cpp
    ContiguousCodestreamContents.cpp
    DataEntryURLBox.cpp
    DataEntryURLContents.cpp
    EnumCS.cpp
    ImageHeaderBox.cpp
    ImageHeaderContents.cpp
    IntellectualPropertyBox.cpp
    UndefinedContents.cpp
    UniversalUniqueIdentifier.cpp
    UUIDBox.cpp
    UUIDContents.cpp
    UUIDInfoBox.cpp
    UUIDInfoContents.cpp
    UUIDListBox.cpp
    UUIDListContents.cpp
    XMLContents.cpp
    XMLBox.cpp)

add_library(jplm_common_boxes_generic ${COMMON_BOXES_GENERIC_SOURCES})