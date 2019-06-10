/*
* @Author: Ismael Seidel
* @Date:   2019-06-04 11:47:31
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-06-10 12:27:12
*/
#include <iostream>
#include "Lib/Part2/Common/LightfieldIOConfiguration.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";

TEST(InitializationOfLighfieldIOConfiguration,
    InitializationSetSizeForInitialZero) {
  LightfieldDimension<std::size_t> size(2, 3, 4, 5);
  LightfieldCoordinate<std::size_t> initial(0, 0, 0, 0);

  LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, size);
  EXPECT_EQ(lfc.get_size(), size);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationSetSizeForInitialNotZero) {
  LightfieldDimension<std::size_t> size(2, 3, 4, 5);
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);

  LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, size);
  EXPECT_EQ(lfc.get_size(), size);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithInitialAndFinalCoordinatesGivesCorrectSize) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  LightfieldCoordinate<std::size_t> expected_size(12, 14, 16, 18);

  LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final);
  EXPECT_EQ(lfc.get_size(), expected_size);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithFinalCoordinatesThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> final(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> initial(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithTThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(18, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(6, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithSThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 21, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 7, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithVThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 24, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 8, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithUThatGenerateNegativeSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 27);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 9);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldCoordinateExceptions::UnsignedUnderflowException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithTThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(18, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithSThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 21, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithVThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 24, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithUThatGenerateZeroSizeThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 27);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithEqualCoordinatesThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, initial),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithInvalidPathThrows) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_griik"}, initial, final),
      LightfieldIOConfigurationExceptions::InvalidLightfieldPath);
}


TEST(InitializationOfLighfieldIOConfiguration,
    InitializationWithValidPathAndSizesDoesNotThrow) {
  LightfieldCoordinate<std::size_t> initial(6, 7, 8, 9);
  LightfieldCoordinate<std::size_t> final(18, 21, 24, 27);
  EXPECT_NO_THROW(LightfieldIOConfiguration lfc({resources_path+"/small_greek"}, initial, final));
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}