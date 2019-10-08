#setting up the compiler warnings... 
if(CMAKE_COMPILER_IS_GNUCXX)
	#appending gcc flags
	if(CMAKE_BUILD_TYPE MATCHES DEBUG)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
	else(CMAKE_BUILD_TYPE MATCHES DEBUG) 
	    message(STATUS "Adopting optimizations -O2 -march=native")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native")
	endif(CMAKE_BUILD_TYPE MATCHES DEBUG)
else()
	message("The compiler is not gcc. Please configure the proper warnings in cmake/JPLMWarnings.cmake")
endif()