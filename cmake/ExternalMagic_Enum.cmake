message("Configuring magic_enum library.")

#   # Download and unpack magic_enum at configure time
configure_file(cmake/CMakeLists.magic_enum.txt.in ${CMAKE_CURRENT_BINARY_DIR}/magic_enum-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/magic_enum-download )
if(result)
  message(FATAL_ERROR "CMake step for magic_enum failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/magic_enum-download )
if(result)
  message(FATAL_ERROR "Build step for magic_enum failed: ${result}")
endif()


set(magic_enum_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/magic_enum-src/magic_enum/include)

include_directories(${magic_enum_INCLUDE})
