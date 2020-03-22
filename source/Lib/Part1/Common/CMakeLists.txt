set(PART1_COMMON_SOURCES
    CommonExceptions.cpp
    CatalogGenerator.cpp
    JPLFile.cpp
    JPLMCodec.cpp
    ../../Common/Boxes/Box.cpp
    ../../Common/Boxes/InFileDBox.cpp
    ../../Common/Boxes/InMemoryDBox.cpp)

add_library(jplm_part1_common ${PART1_COMMON_SOURCES})