
set(METRICS_TESTS_SOURCES "MetricsTests.cpp")
add_executable(metrics_tests ${METRICS_TESTS_SOURCES})
target_link_libraries(metrics_tests PRIVATE gtest_main)
target_link_libraries(metrics_tests PRIVATE image)

set(IMAGE_CHANNEL_TESTS_SOURCES "ImageChannelTests.cpp")
add_executable(image_channel_tests ${IMAGE_CHANNEL_TESTS_SOURCES})
target_link_libraries(image_channel_tests PRIVATE gtest_main)
target_link_libraries(image_channel_tests PRIVATE image)