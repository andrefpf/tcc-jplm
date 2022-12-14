set(COMMON_SOURCES JPLMCodecFactory.cpp
    JPLMConfiguration.cpp
    JPLMEncoderConfigurationLightField.cpp
    JPLMEncoderConfigurationLightField4DTransformMode.cpp
    JPLMEncoderConfigurationLightField4DPredictionMode.cpp
    JPLMEncoderConfiguration.cpp
    JPLMDecoderConfiguration.cpp
    JPLMConfigurationFactory.cpp
    Boxes/Box.cpp)

set(PART1_SOURCES
    ../Part1/Common/JPLFile.cpp
    ../Part1/Common/CatalogGenerator.cpp)

set(PART2_SOURCES
    ../Part2/Encoder/TransformMode/Hierarchical4DEncoder.cpp
    ../Part2/Encoder/TransformMode/TransformPartition.cpp
    ../Part2/Encoder/TransformMode/ABACEncoder.cpp
    ../Part2/Encoder/TransformMode/JPLM4DTransformModeLightFieldEncoder.cpp
    ../Part2/Decoder/TransformMode/PartitionDecoder.cpp
    ../Part2/Decoder/TransformMode/Hierarchical4DDecoder.cpp
    ../Part2/Decoder/TransformMode/JPLM4DTransformModeLightFieldDecoder.cpp
    ../Part2/Decoder/TransformMode/CodestreamPointerSetMarkerSegmentParser.cpp
    ../Part2/Decoder/TransformMode/ABACDecoder.cpp)

add_library(jplm_common ${COMMON_SOURCES} ${PART1_SOURCES} ${PART2_SOURCES})
target_link_libraries(jplm_common basic_configuration jplm_part2_common_transform_mode image)
