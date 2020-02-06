add_jplm_test(UniversalUniqueIdentifierTests
              UniversalUniqueIdentifier_tests
              UniversalUniqueIdentifierTests.cpp
              "gtest_main;jplm_part1_common")

add_jplm_test(UUIDBoxTests UUIDBox_tests UUIDBoxTests.cpp "gtest_main;jplm_part1_common")

add_jplm_test(ImageHeaderContentsTests
              image_header_contents_tests
              ImageHeaderContentsTests.cpp
              "gtest_main;jplm_part1_common;jplm_common_boxes_generic;jplm_common_boxes")

add_jplm_test(ImageHeaderBoxTests
              image_header_box_tests
              ImageHeaderBoxTests.cpp
              "gtest_main;jplm_part1_common;jplm_common_boxes_generic;jplm_common_boxes")