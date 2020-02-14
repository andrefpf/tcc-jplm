set(PART2_DECODER_TRANSFORM_MODE_SOURCES
    ABACDecoder.cpp
    CodestreamPointerSetMarkerSegmentParser.cpp
    ColourComponentScalingMarkerSegmentParser.cpp
    CommonExceptions.cpp
    Hierarchical4DDecoder.cpp
    JPLM4DTransformModeLightFieldDecoder.cpp
    LightFieldContigurationMarkerSegmentParser.cpp
    MarkerSegmentHelper.cpp
    PartitionDecoder.cpp)

add_library(jplm_part2_decoder_transform_mode ${PART2_DECODER_TRANSFORM_MODE_SOURCES})