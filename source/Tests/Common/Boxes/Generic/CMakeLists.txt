function(add_jplm_part1_common_lib_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part1_common)
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()


add_jplm_part1_common_lib_test(UniversalUniqueIdentifierTests UniversalUniqueIdentifier_tests UniversalUniqueIdentifierTests.cpp)
add_jplm_part1_common_lib_test(UUIDBoxTests UUIDBox_tests UUIDBoxTests.cpp)
add_jplm_part1_common_lib_test(ImageHeaderContentsTests image_header_contents_tests ImageHeaderContentsTests.cpp)
add_jplm_part1_common_lib_test(ImageHeaderBoxTests image_header_box_tests ImageHeaderBoxTests.cpp)
