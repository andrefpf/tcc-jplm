add_jplm_test(JPLFileFromStreamTests jpl_file_from_stream_part2_tests
              JPLFileFromStreamTests.cpp
              "gtest_main;stream;jplm_common_boxes_parsers;jplm_part2_boxes_decoder;jplm_common;jplm_part1_decoder;jplm_part2_common_transform_mode")

add_jplm_test(JPLM4DTransformModeLightFieldDecoderTests
              jplm_4d_transform_mode_light_field_decoder_tests
              JPLM4DTransformModeLightFieldDecoderTests.cpp
              "gtest_main;stream;jplm_common_boxes_parsers;jplm_part2_boxes_decoder;jplm_common;jplm_part1_decoder;jplm_part2_common_transform_mode")

target_sources(jplm_4d_transform_mode_light_field_decoder_tests PRIVATE "${CMAKE_SOURCE_DIR}/source/Lib/Common/JPLMConfiguration.cpp")
