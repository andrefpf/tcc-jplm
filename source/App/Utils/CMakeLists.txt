set(JPLM_UTILS_PATH "${EXECUTABLE_OUTPUT_PATH}/utils/")

function(add_jplm_util UTIL_NAME UTIL_SOURCE LIB_NAMES)
  add_executable(
      ${UTIL_NAME} ${UTIL_SOURCE})
  foreach (lib_to_link ${LIB_NAMES})
    message(STATUS
        "Including link library ${lib_to_link}")
    target_link_libraries(${UTIL_NAME} PRIVATE ${lib_to_link})
  endforeach ()
  set_target_properties(${UTIL_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
      "${JPLM_UTILS_PATH}")
endfunction()


add_jplm_util(
    lightfield_coordinate_shift LightFieldViewCoordinateChange.cpp
    "image;jplm_part2_common;basic_configuration")

add_jplm_util(lenslet_13x13_shifter
              Lenslet13x13Shifter.cpp
              "image;stream")


add_jplm_util(compute_psnr ComputePSNR.cpp "image;stream")


if (VISUALIZATION_TOOL)
  add_jplm_util(
      lightfield_visualizer LightfieldVisualization.cpp
      "image;stream;jplm_part2_common;basic_configuration;X11")
endif (VISUALIZATION_TOOL)

add_jplm_util(
    compute_lightfield_quality_metrics ComputeLightFieldQualityMetrics.cpp
    "image;stream;jplm_part2_common;basic_configuration")

add_jplm_util(convert_ppm_to_pgx PPMToPGX.cpp
    "image;stream;basic_configuration")

add_jplm_util(convert_pgx_to_ppm
              PGXToPPM.cpp
              "image;stream;basic_configuration")
