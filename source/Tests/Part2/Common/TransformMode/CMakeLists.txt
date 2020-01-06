function(add_jplm_part2_common_transform_mode_lib_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_common)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_common_boxes)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_common_boxes_generic)	
	target_link_libraries(${TEST_BIN_NAME} PRIVATE image)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE stream)
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()


add_jplm_part2_common_transform_mode_lib_test(Block4DTests block4d_tests Block4DTests.cpp)
add_jplm_part2_common_transform_mode_lib_test(MarkersTests markers_tests MarkersTests.cpp)