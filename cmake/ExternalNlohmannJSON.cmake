message("Configuring json library.")

#   # Download and unpack json at configure time
configure_file(cmake/CMakeLists.NlohmannJSON.txt.in ${CMAKE_CURRENT_BINARY_DIR}/nlohmannjson-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nlohmannjson-download )
if(result)
  message(FATAL_ERROR "CMake step for json failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/nlohmannjson-download )
if(result)
  message(FATAL_ERROR "Build step for json failed: ${result}")
endif()


set(json_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/nlohmannjson-src/json/single_include)

include_directories(${json_INCLUDE})
