add_jplm_test(ColourSpecificationBoxParserTests
              colour_specification_box_parser_tests
              ColourSpecificationBoxParserTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;stream;jplm_common_boxes_parsers;")


add_jplm_test(ContiguousCodestreamBoxParserTests
              contiguous_codestream_box_parser_tests
              ContiguousCodestreamBoxParserTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;stream;jplm_common_boxes_parsers;")