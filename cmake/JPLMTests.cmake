set(JPLM_TESTS_PATH "${EXECUTABLE_OUTPUT_PATH}/tests/")

function(add_jplm_test TEST_NAME TEST_BIN_NAME TEST_SOURCE LIB_NAMES)
	message(STATUS "Configuring a new test ${TEST_NAME}")
	add_executable(${TEST_BIN_NAME} ${TEST_SOURCE})
	foreach(lib_to_link ${LIB_NAMES})
		message(STATUS "Including link library ${lib_to_link}")
		target_link_libraries(${TEST_BIN_NAME} PRIVATE ${lib_to_link})
	endforeach()
	set_target_properties( ${TEST_BIN_NAME}
	    PROPERTIES
	    RUNTIME_OUTPUT_DIRECTORY "${JPLM_TESTS_PATH}"
	)
	add_test(${TEST_NAME} ${JPLM_TESTS_PATH}/${TEST_BIN_NAME} "${CMAKE_HOME_DIRECTORY}/resources")
endfunction()