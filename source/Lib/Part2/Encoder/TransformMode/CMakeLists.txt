set(PART2_ENCODER_TRANSFORM_MODE_SOURCES
	CommonExceptions.cpp
    ABACEncoder.cpp
    Hierarchical4DEncoder.cpp
    TransformPartition.cpp
    JPLM4DTransformModeLightFieldEncoder.cpp
    RDCostResult.cpp
    ../../../Common/JPLMEncoderConfigurationLightField4DTransformMode.cpp)

add_library(jplm_part2_encoder_transform_mode ${PART2_ENCODER_TRANSFORM_MODE_SOURCES})

target_link_libraries(jplm_part2_encoder_transform_mode jplm_part2_common_transform_mode)