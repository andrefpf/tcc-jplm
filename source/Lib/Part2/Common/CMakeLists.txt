set(PART2_COMMON_SOURCES
    CommonExceptions.cpp
    JPLMLightFieldCodec.cpp
    LightfieldCoordinate.cpp
    Lightfield.cpp
    LightfieldDimension.cpp
    LightfieldFromFile.cpp
    LightfieldIOConfiguration.cpp
    PGX3CharViewToFilename.cpp
    PPM3CharViewToFilename.cpp
    View.cpp
    ViewFromPGXFile.cpp
    ViewIOPolicy.cpp
    ViewIOPolicyLimitedMemory.cpp
    ViewIOPolicyLimitedNumberOfViews.cpp
    ViewIOPolicyLimitlessMemory.cpp
    ViewIOPolicyOneAtATime.cpp
    ViewIOPolicyQueue.cpp
    ViewToFilenameTranslator.cpp)

add_library(jplm_part2_common ${PART2_COMMON_SOURCES})