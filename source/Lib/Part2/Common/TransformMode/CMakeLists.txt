set(PART2_COMMON_TRANSFORM_MODE_SOURCES
    ABACCodec.cpp
    Block4D.cpp
    BorderBlocksPolicy.cpp
    CodestreamPointerSetMarkerSegment.cpp
    ColourComponentScalingMarkerSegment.cpp
    ComponentSsizParameter.cpp
    DCT4DBlock.cpp
    DCT4DCoefficientsManager.cpp
    Hierarchical4DCodec.cpp
    JPLM4DTransformModeLightFieldCodec.cpp
    LightFieldConfigurationMarkerSegment.cpp
    LightFieldTransformMode.cpp
    Markers.cpp
    ProbabilityModel.cpp
    Transformed4DBlock.cpp)


add_library(jplm_part2_common_transform_mode ${PART2_COMMON_TRANSFORM_MODE_SOURCES})

target_link_libraries(jplm_part2_common_transform_mode
                      jplm_common_boxes_generic jplm_part2_common image)