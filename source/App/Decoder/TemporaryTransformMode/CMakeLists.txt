set(TEMP_TRANSFORM_MODE_DECODER_SOURCES decoder.cpp MuleDecoder.cpp ../../TemporaryCommonTransformMode/MuleCodec.cpp ../../TemporaryCommonTransformMode/ParameterHandler.cpp)

add_executable(jpl-temp-decoder-transform-mode-bin ${TEMP_TRANSFORM_MODE_DECODER_SOURCES})
target_link_libraries(jpl-temp-decoder-transform-mode-bin PRIVATE jplm_part2_decoder_transform_mode)

# target_link_libraries(jpl-encoder-bin PRIVATE jplm_part1_common)