add_jplm_test(JpegPlenoSignatureBoxParserTests
              jpeg_pleno_signature_box_parser_tests
              JpegPlenoSignatureBoxParserTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;jplm_common_boxes_parsers;stream;jplm_part1_common_boxes")

add_jplm_test(FileTypeBoxParserTests
              file_type_box_parser_tests
              FileTypeBoxParserTests.cpp
              "gtest_main;jplm_part2_boxes_decoder;jplm_common_boxes_parsers;stream;jplm_part1_common_boxes")