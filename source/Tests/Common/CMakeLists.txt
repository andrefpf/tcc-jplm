add_jplm_test(JPLFileIOTests jpl_file_io_tests
              JPLFileIOTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;jplm_common_boxes_parsers;stream;jplm_common;jplm_part1_decoder;jplm_part1_common_boxes;jplm_common_boxes_parsers")

add_jplm_test(JPLMCodecFactoryTests jplm_codec_factory_tests
              JPLMCodecFactoryTests.cpp "gtest_main;jplm_part2_boxes_decoder;jplm_common_boxes_parsers;stream;jplm_common;jplm_part1_common_boxes;jplm_common_boxes_parsers")

target_sources(jplm_codec_factory_tests PRIVATE "${CMAKE_SOURCE_DIR}/source/Lib/Common/JPLMConfiguration.cpp")



add_jplm_test(JPLMConfigurationFactoryTests jplm_configuration_factory_tests
              JPLMConfigurationFactoryTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;jplm_common_boxes_parsers;stream;jplm_common;jplm_part1_common_boxes;jplm_common_boxes_parsers")
target_sources(jplm_configuration_factory_tests PRIVATE "${CMAKE_SOURCE_DIR}/source/Lib/Common/JPLMConfiguration.cpp")

add_jplm_test(JPLMEncoderConfigurationTests
              jplm_encoder_configuration_tests
              JPLMEncoderConfigurationTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;jplm_common_boxes_parsers;stream;jplm_common")

target_sources(jplm_encoder_configuration_tests
               PRIVATE
               "${CMAKE_SOURCE_DIR}/source/Lib/Common/JPLMConfiguration.cpp")
