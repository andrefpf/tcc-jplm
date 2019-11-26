function(add_jplm_part2_encoder_lib_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part1_common)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_encoder)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_common)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE image)
	target_sources(${TEST_BIN_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/source/Lib/Common/JPLMConfiguration.cpp")
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()


add_jplm_part2_encoder_lib_test(JPLM4DTransformModeLightFieldEncoderTests jplm_4d_transform_mode_light_field_encoder_tests JPLM4DTransformModeLightFieldEncoderTests.cpp)
