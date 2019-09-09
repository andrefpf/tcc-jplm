#setting up the compiler warnings... 
if(CMAKE_COMPILER_IS_GNUCXX)
	#appending gcc flags
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra")
	add_definitions(-Wfatal-errors)
else()
	message("The compiler is not gcc. Please configure the proper warnings in cmake/JPLMWarnings.cmake")
endif()