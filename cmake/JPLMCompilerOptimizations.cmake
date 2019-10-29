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
else()
	message("The compiler is not gcc. Please configure the proper warnings in cmake/JPLMWarnings.cmake")
endif()