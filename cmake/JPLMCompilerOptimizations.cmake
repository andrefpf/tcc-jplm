#setting up the compiler warnings... 
if(CMAKE_COMPILER_IS_GNUCXX)
	#appending gcc flags
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		message(STATUS "Debug mode on")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
	else(CMAKE_BUILD_TYPE MATCHES Debug) 
	    message(STATUS "Not on debug mode...")
	    message(STATUS "Adopting optimizations -O3 -march=native")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")
	endif(CMAKE_BUILD_TYPE MATCHES Debug)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	message(STATUS "Adopting optimizations ")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /Gy-")
else()
	message("The compiler is not supported. Please configure the proper warnings in cmake/JPLMCompilerOptimizations.cmake")
endif()