/*
* @Author: Ismael Seidel
* @Date:   2019-06-04 11:47:31
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-06-26 16:18:31
*/
#include <iostream>
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";

TEST(InitializationOfLighfieldIOConfiguration,
    InitializationSetSizeForInitialZero) {
  LightfieldDimension<std::size_t> size(2, 3, 4, 5);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);

  LightfieldIOConfiguration lfc(
      {resources_path + "/small_greek"}, size, initial);
  EXPECT_EQ(lfc.get_size(), size);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationSetSizeForInitialNotZero) {
  LightfieldDimension<std::size_t> size(2, 3, 4, 5);
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);

  LightfieldIOConfiguration lfc(
      {resources_path + "/small_greek"}, size, initial);
  EXPECT_EQ(lfc.get_size(), size);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithInitialAndFinalCoordinatesGivesCorrectSize) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  LightfieldCoordinate<std::size_t> expected_size(12, 14, 16, 18);

  LightfieldIOConfiguration lfc(
      {resources_path + "/small_greek"}, initial, final);
  EXPECT_EQ(lfc.get_size(), expected_size);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithFinalCoordinatesThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> final(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> initial(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithTThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(18, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(6, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithSThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 21, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 7, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithVThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 24, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 8, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithUThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 27);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 9);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithTThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(18, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithSThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 21, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithVThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 24, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithUThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 27);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithEqualCoordinatesThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_greek"}, initial, initial),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithInvalidPathThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc(
                   {resources_path + "/small_griik"}, initial, final),
      LightfieldIOConfigurationExceptions::InvalidLightfieldPath);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithValidPathAndSizesDoesNotThrow) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_NO_THROW(LightfieldIOConfiguration lfc(
      {resources_path + "/small_greek"}, initial, final));
}


struct LightfieldIOConfigurationGetters : public testing::Test {
 protected:
  LightfieldCoordinate<std::size_t> initial;
  LightfieldCoordinate<std::size_t> final;
  LightfieldIOConfiguration lfc;

 public:
  LightfieldIOConfigurationGetters()
      : initial(6, 7, 8, 9), final(18, 21, 24, 27),
        lfc({resources_path + "/small_greek"}, initial, final){};
  ~LightfieldIOConfigurationGetters() = default;
};


TEST_F(LightfieldIOConfigurationGetters, KnowsTheNumberOfPixelsPerLightfield) {
  auto number_of_pixels = 48384;  //12*14
  auto number_of_pixel_getter = lfc.get_number_of_pixels_per_lightfield();
  EXPECT_EQ(number_of_pixels, number_of_pixel_getter);
}


TEST_F(LightfieldIOConfigurationGetters, KnowsTheNumberOfPixelsPerView) {
  auto number_of_pixels = 288;  //12*14
  auto number_of_pixel_getter = lfc.get_number_of_pixels_per_view();
  EXPECT_EQ(number_of_pixels, number_of_pixel_getter);
}


TEST_F(LightfieldIOConfigurationGetters, KnowsTheNumberOfViews) {
  auto number_of_views = 168;  //12*14
  auto number_of_view_getter = lfc.get_number_of_views_per_lightfield();
  EXPECT_EQ(number_of_views, number_of_view_getter);
}


TEST_F(LightfieldIOConfigurationGetters,
    GeneratesAListOfCoordinatesForViewsWithRightSize) {
  auto number_of_views = 168;  //12*14
  auto number_of_view_coordinates = lfc.get_raster_view_coordinates().size();
  EXPECT_EQ(number_of_views, number_of_view_coordinates);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}