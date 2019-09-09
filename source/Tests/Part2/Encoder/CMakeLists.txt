function(add_jplm_part2_encoder_lib_test TEST_NAME TEST_BIN_NAME TEST_SOURCE)
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	target_link_libraries(${TEST_BIN_NAME} PRIVATE gtest_main)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part1_common)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_encoder)
	target_link_libraries(${TEST_BIN_NAME} PRIVATE jplm_part2_common)	
	target_link_libraries(${TEST_BIN_NAME} PRIVATE image)	
	add_test(${TEST_NAME} ${EXECUTABLE_OUTPUT_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()


add_jplm_part2_encoder_lib_test(JpegPlenoLightFieldEncoder4DTransformModeTests jpeg_pleno_light_field_encoder_4d_transform_mode_tests JpegPlenoLightFieldEncoder4DTransformModeTests.cpp)
