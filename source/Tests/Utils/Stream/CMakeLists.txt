function(add_stream_lib_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE stream)
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()

add_stream_lib_test(BinaryToolsTests binary_tools_tests BinaryToolsTests.cpp)
add_stream_lib_test(ManagedStreamTests managed_stream_tests ManagedStreamTests.cpp)