add_jplm_test(JPLM4DTransformModeLightFieldEncoderTests
              jplm_4d_transform_mode_light_field_encoder_tests
              JPLM4DTransformModeLightFieldEncoderTests.cpp
              "gtest_main;jplm_part1_common;jplm_part2_encoder;jplm_part2_common;image;jplm_common")
# target_sources(jplm_4d_transform_mode_light_field_encoder_tests PRIVATE "${CMAKE_SOURCE_DIR}/source/Lib/Common/JPLMConfiguration.cpp")
