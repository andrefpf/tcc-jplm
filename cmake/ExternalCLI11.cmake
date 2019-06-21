message("Configuring CLI11 library.")

#   # Download and unpack CLI11 at configure time
configure_file(cmake/CMakeLists.CLI11.txt.in ${CMAKE_CURRENT_BINARY_DIR}/CLI11-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CLI11-download )
if(result)
  message(FATAL_ERROR "CMake step for CLI11 failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/CLI11-download )
if(result)
  message(FATAL_ERROR "Build step for CLI11 failed: ${result}")
endif()


set(CLI11_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/CLI11-src/CLI11/include)

include_directories(${CLI11_INCLUDE})
