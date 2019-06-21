message("Configuring CPPItertools library.")

#   # Download and unpack cppitertools at configure time
configure_file(cmake/CMakeLists.cppitertools.txt.in ${CMAKE_CURRENT_BINARY_DIR}/cppitertools-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cppitertools-download )
if(result)
  message(FATAL_ERROR "CMake step for cppitertools failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/cppitertools-download )
if(result)
  message(FATAL_ERROR "Build step for cppitertools failed: ${result}")
endif()


set(CPPItertools_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/cppitertools-src)

include_directories(${CPPItertools_INCLUDE})
