message("Configuring openJPEG library.")

include(ExternalProject)
ExternalProject_Add(
    openjpeg
    GIT_REPOSITORY https://github.com/uclouvain/openjpeg.git
    GIT_TAG master
    GIT_PROGRESS    1
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/thirdparty/openjpeg -DOPENJPEG_INSTALL_INCLUDE_DIR=include
    PREFIX openjpeg
    UPDATE_COMMAND ""
    )

set(openjpeg_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/openjpeg/lib/)
link_directories(${openjpeg_LIB_DIR})
set(openjpeg_LIB ${CMAKE_STATIC_LIBRARY_PREFIX}openjp2${CMAKE_STATIC_LIBRARY_SUFFIX})
set(openjpeg_INCLUDE ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/openjpeg/include)

# include_directories(${openjpeg_INCLUDE})