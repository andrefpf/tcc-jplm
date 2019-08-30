function(add_jplm_io_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_boxes_decoder)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_common_boxes_parsers)	
	target_link_libraries(${TEST_BIN_NAME} PRIVATE stream)
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()


add_jplm_io_test(JPLFileIOTests jpl_file_io_tests JPLFileIOTests.cpp)
