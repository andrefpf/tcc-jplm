set(PART2_COMMON_BOX_SOURCES
    CameraParametersArray.cpp
    CameraParameterBox.cpp
    CameraParameterContents.cpp
    JpegPlenoLightFieldBox.cpp
    JpegPlenoLightFieldContents.cpp
    JpegPlenoLightFieldHeaderBox.cpp
    JpegPlenoLightFieldHeaderContents.cpp)

add_library(jplm_part2_common_boxes ${PART2_COMMON_BOX_SOURCES})