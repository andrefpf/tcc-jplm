add_jplm_test(Block4DTests block4d_tests
              Block4DTests.cpp
              "gtest_main;jplm_part2_common;jplm_common_boxes;jplm_common_boxes_generic;jplm_part2_common_transform_mode;image;stream")


add_jplm_test(CodestreamPointerSetMarkerSegmentTests
              codestream_pointer_set_marker_segment_tests
              CodestreamPointerSetMarkerSegmentTests.cpp
              "gtest_main;jplm_part2_common;jplm_common_boxes;jplm_common_boxes_generic;jplm_part2_common_transform_mode;image;stream;jplm_part2_decoder_transform_mode")


add_jplm_test(ColourComponentScalingMarkerSegmentTests colour_component_scaling_marker_segment_tests ColourComponentScalingMarkerSegmentTests.cpp "gtest_main;jplm_part2_common;jplm_common_boxes;jplm_common_boxes_generic;jplm_part2_common_transform_mode;image;stream")