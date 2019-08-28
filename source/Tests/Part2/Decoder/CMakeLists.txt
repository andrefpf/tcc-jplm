function(add_jplm_part2_decoder_lib_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE stream)	
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_boxes_part1_decoder)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_boxes_decoder)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_common_boxes_parsers)
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()


add_jplm_part2_decoder_lib_test(JPLFileFromStreamTests jpl_file_from_stream_part2_tests JPLFileFromStreamTests.cpp)
