set(TEMP_TRANSFORM_MODE_ENCODER_SOURCES MuleEncoder.cpp  ../../TemporaryCommonTransformMode/ColorSpaces.cpp ../../TemporaryCommonTransformMode/MuleCodec.cpp ../../TemporaryCommonTransformMode/ParameterHandler.cpp ../../TemporaryCommonTransformMode/ThreeChannelBlock4DHolder.cpp encoder.cpp)

add_executable(jpl-temp-encoder-transform-mode-bin ${TEMP_TRANSFORM_MODE_ENCODER_SOURCES})
target_link_libraries(jpl-temp-encoder-transform-mode-bin PRIVATE jplm_part2_encoder_transform_mode)

# target_link_libraries(jpl-encoder-bin PRIVATE jplm_part1_common)