/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     LightfieldIOConfigurationTests.cpp
 *  \brief    Test of LightfieldIOConfiguration.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-04
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