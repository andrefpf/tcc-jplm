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

/** \file     ImageMetricTests.cpp
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-30
 */


#include <iostream>
#include "Image.h"
#include "ImageMetrics.h"
#include "PPMBinaryFile.h"
#include "PixelMapFileIO.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";

struct SmallRGBImage : testing::Test {
 protected:
  RGBImage<uint16_t> rgb_image_a;
  SmallRGBImage() : rgb_image_a(2, 2, 10) {
    // rgb_image_a = RGBImage<uint16_t>(2, 2, 10);
    std::tuple<uint16_t, uint16_t, uint16_t> color = {0, 0, 0};
    rgb_image_a.set_pixel_at(color, 0, 0);
    rgb_image_a.set_pixel_at(color, 0, 1);
    rgb_image_a.set_pixel_at(color, 1, 0);
    rgb_image_a.set_pixel_at(color, 1, 1);
  }
};


TEST_F(SmallRGBImage, MSEThrowsExceptionForDifferentWidthImages) {
  auto rgb_different_width = RGBImage<uint16_t>(4, 2, 10);
  EXPECT_THROW(ImageMetrics::get_mse(rgb_image_a, rgb_different_width),
      MetricsExceptions::DifferentWidthImagesException);
}


TEST_F(SmallRGBImage, MSEThrowsExceptionForDifferentHeightImages) {
  auto rgb_different_height = RGBImage<uint16_t>(2, 3, 10);
  EXPECT_THROW(ImageMetrics::get_mse(rgb_image_a, rgb_different_height),
      MetricsExceptions::DifferentHeightImagesException);
}


TEST_F(SmallRGBImage, MSEThrowsExceptionForDifferentSizeImages) {
  auto rgb_different_size = RGBImage<uint16_t>(3, 3, 10);
  EXPECT_THROW(ImageMetrics::get_mse(rgb_image_a, rgb_different_size),
      MetricsExceptions::DifferentSizeImagesException);
}


TEST_F(SmallRGBImage, MSEThrowsExceptionForDifferentBppImages) {
  auto rgb_different_bpp = RGBImage<uint16_t>(2, 2, 9);
  EXPECT_THROW(ImageMetrics::get_mse(rgb_image_a, rgb_different_bpp),
      MetricsExceptions::DifferentBppImagesException);
}


TEST_F(SmallRGBImage, MSEThrowsExceptionForDifferentColorSpaces) {
  auto bt_601_image = BT601Image<uint16_t>(2, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, bt_601_image),
      MetricsExceptions::DifferentColorSpaceImagesException);
}

TEST_F(SmallRGBImage, SSEThrowsExceptionForDifferentWidthImages) {
  auto rgb_different_width = RGBImage<uint16_t>(4, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_different_width),
      MetricsExceptions::DifferentWidthImagesException);
}


TEST_F(SmallRGBImage, SSEThrowsExceptionForDifferentHeightImages) {
  auto rgb_different_height = RGBImage<uint16_t>(2, 3, 10);
  EXPECT_THROW(ImageMetrics::get_sum_of_squared_errors(
                   rgb_image_a, rgb_different_height),
      MetricsExceptions::DifferentHeightImagesException);
}


TEST_F(SmallRGBImage, SSEThrowsExceptionForDifferentSizeImages) {
  auto rgb_different_size = RGBImage<uint16_t>(3, 3, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_different_size),
      MetricsExceptions::DifferentSizeImagesException);
}


TEST_F(SmallRGBImage, SSEThrowsExceptionForDifferentBppImages) {
  auto rgb_different_bpp = RGBImage<uint16_t>(2, 2, 9);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_different_bpp),
      MetricsExceptions::DifferentBppImagesException);
}


TEST_F(SmallRGBImage, SSEThrowsExceptionForDifferentColorSpaces) {
  auto bt_601_image = BT601Image<uint16_t>(2, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, bt_601_image),
      MetricsExceptions::DifferentColorSpaceImagesException);
}


TEST_F(SmallRGBImage, PSNRThrowsExceptionForDifferentWidthImages) {
  auto rgb_different_width = RGBImage<uint16_t>(4, 2, 10);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   rgb_image_a, rgb_different_width),
      MetricsExceptions::DifferentWidthImagesException);
}


TEST_F(SmallRGBImage, PSNRThrowsExceptionForDifferentHeightImages) {
  auto rgb_different_height = RGBImage<uint16_t>(2, 3, 10);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   rgb_image_a, rgb_different_height),
      MetricsExceptions::DifferentHeightImagesException);
}


TEST_F(SmallRGBImage, PSNRThrowsExceptionForDifferentSizeImages) {
  auto rgb_different_size = RGBImage<uint16_t>(3, 3, 10);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   rgb_image_a, rgb_different_size),
      MetricsExceptions::DifferentSizeImagesException);
}


TEST_F(SmallRGBImage, PSNRThrowsExceptionForDifferentBppImages) {
  auto rgb_different_bpp = RGBImage<uint16_t>(2, 2, 9);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   rgb_image_a, rgb_different_bpp),
      MetricsExceptions::DifferentBppImagesException);
}


TEST_F(SmallRGBImage, PSNRThrowsExceptionForDifferentColorSpaces) {
  auto bt_601_image = BT601Image<uint16_t>(2, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, bt_601_image),
      MetricsExceptions::DifferentColorSpaceImagesException);
}


TEST_F(SmallRGBImage, RGBSameImageEqualSSEZero) {
  for (const auto& sse :
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_a)) {
    EXPECT_DOUBLE_EQ(sse, 0.0);
  }
}


TEST_F(SmallRGBImage, RGBCopiedImageEqualSSEZero) {
  auto rgb_image_b = rgb_image_a;  //copy
  for (const auto& sse :
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_b)) {
    EXPECT_DOUBLE_EQ(sse, 0.0);
  }
}


TEST_F(SmallRGBImage, RGBChangedOnePixelImageEqualSSEOne) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 1, 1};
  rgb_image_b.set_pixel_at(new_color, 1, 1);

  for (const auto& sse :
      ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_b)) {
    EXPECT_DOUBLE_EQ(sse, 1.0);
  }
}


TEST_F(
    SmallRGBImage, RGBChangedOnePixelRedChannelImageEqualSSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 0};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto sses = ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 1.0);
  EXPECT_DOUBLE_EQ(sses[1], 0.0);
  EXPECT_DOUBLE_EQ(sses[2], 0.0);
}


TEST_F(SmallRGBImage,
    RGBChangedOnePixelGreenChannelImageEqualSSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 1, 0};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto sses = ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 0.0);
  EXPECT_DOUBLE_EQ(sses[1], 1.0);
  EXPECT_DOUBLE_EQ(sses[2], 0.0);
}


TEST_F(
    SmallRGBImage, RGBChangedOnePixelBlueChannelImageEqualSSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 0, 1};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto sses = ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 0.0);
  EXPECT_DOUBLE_EQ(sses[1], 0.0);
  EXPECT_DOUBLE_EQ(sses[2], 1.0);
}


TEST_F(SmallRGBImage,
    RGBChangedOnePixelRedAndBlueChannelImageEqualSSEOneInThoseChannels) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 1};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto sses = ImageMetrics::get_sum_of_squared_errors(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 1.0);
  EXPECT_DOUBLE_EQ(sses[1], 0.0);
  EXPECT_DOUBLE_EQ(sses[2], 1.0);
}


TEST_F(SmallRGBImage, RGBSameImageEqualZeroMSE) {
  for (const auto& mse : ImageMetrics::get_mse(rgb_image_a, rgb_image_a)) {
    EXPECT_DOUBLE_EQ(mse, 0.0);
  }
}


TEST_F(SmallRGBImage, RGBCopiedImageEqualMSEZero) {
  auto rgb_image_b = rgb_image_a;  //copy
  for (const auto& mse : ImageMetrics::get_mse(rgb_image_a, rgb_image_b)) {
    EXPECT_DOUBLE_EQ(mse, 0.0);
  }
}


TEST_F(SmallRGBImage, RGBChangedOnePixelImageEqualMSEOne) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 1, 1};
  rgb_image_b.set_pixel_at(new_color, 1, 1);

  for (const auto& mse : ImageMetrics::get_mse(rgb_image_a, rgb_image_b)) {
    EXPECT_DOUBLE_EQ(mse, 0.25);
  }
}


TEST_F(
    SmallRGBImage, RGBChangedOnePixelRedChannelImageEqualMSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 0};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.25);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallRGBImage,
    RGBChangedOnePixelGreenChannelImageEqualMSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 1, 0};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.25);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(
    SmallRGBImage, RGBChangedOnePixelBlueChannelImageEqualMSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 0, 1};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.25);
}


TEST_F(SmallRGBImage,
    RGBChangedOnePixelRedAndBlueChannelImageEqualMSEOneInThoseChannels) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 1};
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.25);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.25);
}


TEST_F(SmallRGBImage, RGBChangedAllPixelsImageEqualMSEOne) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 1, 1};
  rgb_image_b.set_pixel_at(new_color, 0, 0);
  rgb_image_b.set_pixel_at(new_color, 0, 1);
  rgb_image_b.set_pixel_at(new_color, 1, 0);
  rgb_image_b.set_pixel_at(new_color, 1, 1);

  for (const auto& mse : ImageMetrics::get_mse(rgb_image_a, rgb_image_b)) {
    EXPECT_DOUBLE_EQ(mse, 1.0);
  }
}


TEST_F(
    SmallRGBImage, RGBChangedAllPixelsRedChannelImageEqualMSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 0};
  rgb_image_b.set_pixel_at(new_color, 0, 0);
  rgb_image_b.set_pixel_at(new_color, 0, 1);
  rgb_image_b.set_pixel_at(new_color, 1, 0);
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 1.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallRGBImage,
    RGBChangedAllPixelsGreenChannelImageEqualMSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 1, 0};
  rgb_image_b.set_pixel_at(new_color, 0, 0);
  rgb_image_b.set_pixel_at(new_color, 0, 1);
  rgb_image_b.set_pixel_at(new_color, 1, 0);
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 1.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallRGBImage,
    RGBChangedAllPixelsBlueChannelImageEqualMSEOneInThatChannel) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 0, 1};
  rgb_image_b.set_pixel_at(new_color, 0, 0);
  rgb_image_b.set_pixel_at(new_color, 0, 1);
  rgb_image_b.set_pixel_at(new_color, 1, 0);
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 1.0);
}


TEST_F(SmallRGBImage,
    RGBChangedAllPixelsRedAndBlueChannelImageEqualMSEOneInThoseChannels) {
  auto rgb_image_b = rgb_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 1};
  rgb_image_b.set_pixel_at(new_color, 0, 0);
  rgb_image_b.set_pixel_at(new_color, 0, 1);
  rgb_image_b.set_pixel_at(new_color, 1, 0);
  rgb_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(rgb_image_a, rgb_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 1.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 1.0);
}


struct SmallBT601Image : testing::Test {
 protected:
  BT601Image<uint16_t> bt601_image_a;
  SmallBT601Image() : bt601_image_a(2, 2, 10) {
    // bt601_image_a = BT601Image<uint16_t>(2, 2, 10);
    std::tuple<uint16_t, uint16_t, uint16_t> color = {0, 0, 0};
    bt601_image_a.set_pixel_at(color, 0, 0);
    bt601_image_a.set_pixel_at(color, 0, 1);
    bt601_image_a.set_pixel_at(color, 1, 0);
    bt601_image_a.set_pixel_at(color, 1, 1);
  }
};


TEST_F(SmallBT601Image, MSEThrowsExceptionForDifferentWidthImages) {
  auto bt601_different_width = BT601Image<uint16_t>(4, 2, 10);
  EXPECT_THROW(ImageMetrics::get_mse(bt601_image_a, bt601_different_width),
      MetricsExceptions::DifferentWidthImagesException);
}


TEST_F(SmallBT601Image, MSEThrowsExceptionForDifferentHeightImages) {
  auto bt601_different_height = BT601Image<uint16_t>(2, 3, 10);
  EXPECT_THROW(ImageMetrics::get_mse(bt601_image_a, bt601_different_height),
      MetricsExceptions::DifferentHeightImagesException);
}


TEST_F(SmallBT601Image, MSEThrowsExceptionForDifferentSizeImages) {
  auto bt601_different_size = BT601Image<uint16_t>(3, 3, 10);
  EXPECT_THROW(ImageMetrics::get_mse(bt601_image_a, bt601_different_size),
      MetricsExceptions::DifferentSizeImagesException);
}


TEST_F(SmallBT601Image, MSEThrowsExceptionForDifferentBppImages) {
  auto bt601_different_bpp = BT601Image<uint16_t>(2, 2, 9);
  EXPECT_THROW(ImageMetrics::get_mse(bt601_image_a, bt601_different_bpp),
      MetricsExceptions::DifferentBppImagesException);
}


TEST_F(SmallBT601Image, MSEThrowsExceptionForDifferentColorSpaces) {
  auto rgb_image = RGBImage<uint16_t>(2, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, rgb_image),
      MetricsExceptions::DifferentColorSpaceImagesException);
}


TEST_F(SmallBT601Image, SSEThrowsExceptionForDifferentWidthImages) {
  auto bt601_different_width = BT601Image<uint16_t>(4, 2, 10);
  EXPECT_THROW(ImageMetrics::get_sum_of_squared_errors(
                   bt601_image_a, bt601_different_width),
      MetricsExceptions::DifferentWidthImagesException);
}


TEST_F(SmallBT601Image, SSEThrowsExceptionForDifferentHeightImages) {
  auto bt601_different_height = BT601Image<uint16_t>(2, 3, 10);
  EXPECT_THROW(ImageMetrics::get_sum_of_squared_errors(
                   bt601_image_a, bt601_different_height),
      MetricsExceptions::DifferentHeightImagesException);
}


TEST_F(SmallBT601Image, SSEThrowsExceptionForDifferentSizeImages) {
  auto bt601_different_size = BT601Image<uint16_t>(3, 3, 10);
  EXPECT_THROW(ImageMetrics::get_sum_of_squared_errors(
                   bt601_image_a, bt601_different_size),
      MetricsExceptions::DifferentSizeImagesException);
}


TEST_F(SmallBT601Image, SSEThrowsExceptionForDifferentBppImages) {
  auto bt601_different_bpp = BT601Image<uint16_t>(2, 2, 9);
  EXPECT_THROW(ImageMetrics::get_sum_of_squared_errors(
                   bt601_image_a, bt601_different_bpp),
      MetricsExceptions::DifferentBppImagesException);
}


TEST_F(SmallBT601Image, SSEThrowsExceptionForDifferentColorSpaces) {
  auto rgb_image = RGBImage<uint16_t>(2, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, rgb_image),
      MetricsExceptions::DifferentColorSpaceImagesException);
}


TEST_F(SmallBT601Image, PSNRThrowsExceptionForDifferentWidthImages) {
  auto bt601_different_width = BT601Image<uint16_t>(4, 2, 10);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   bt601_image_a, bt601_different_width),
      MetricsExceptions::DifferentWidthImagesException);
}


TEST_F(SmallBT601Image, PSNRThrowsExceptionForDifferentHeightImages) {
  auto bt601_different_height = BT601Image<uint16_t>(2, 3, 10);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   bt601_image_a, bt601_different_height),
      MetricsExceptions::DifferentHeightImagesException);
}


TEST_F(SmallBT601Image, PSNRThrowsExceptionForDifferentSizeImages) {
  auto bt601_different_size = BT601Image<uint16_t>(3, 3, 10);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   bt601_image_a, bt601_different_size),
      MetricsExceptions::DifferentSizeImagesException);
}


TEST_F(SmallBT601Image, PSNRThrowsExceptionForDifferentBppImages) {
  auto bt601_different_bpp = BT601Image<uint16_t>(2, 2, 9);
  EXPECT_THROW(ImageMetrics::get_peak_signal_to_noise_ratio(
                   bt601_image_a, bt601_different_bpp),
      MetricsExceptions::DifferentBppImagesException);
}


TEST_F(SmallBT601Image, PSNRThrowsExceptionForDifferentColorSpaces) {
  auto rgb_image = RGBImage<uint16_t>(2, 2, 10);
  EXPECT_THROW(
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, rgb_image),
      MetricsExceptions::DifferentColorSpaceImagesException);
}


TEST_F(SmallBT601Image, RGBSameImageEqualSSEZero) {
  for (const auto& sse :
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_a)) {
    EXPECT_DOUBLE_EQ(sse, 0.0);
  }
}


TEST_F(SmallBT601Image, RGBCopiedImageEqualSSEZero) {
  auto bt601_image_b = bt601_image_a;  //copy
  for (const auto& sse :
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_b)) {
    EXPECT_DOUBLE_EQ(sse, 0.0);
  }
}


TEST_F(SmallBT601Image, RGBChangedOnePixelImageEqualSSEOne) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 1, 1};
  bt601_image_b.set_pixel_at(new_color, 1, 1);

  for (const auto& sse :
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_b)) {
    EXPECT_DOUBLE_EQ(sse, 1.0);
  }
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelRedChannelImageEqualSSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 0};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto sses =
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 1.0);
  EXPECT_DOUBLE_EQ(sses[1], 0.0);
  EXPECT_DOUBLE_EQ(sses[2], 0.0);
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelGreenChannelImageEqualSSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 1, 0};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto sses =
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 0.0);
  EXPECT_DOUBLE_EQ(sses[1], 1.0);
  EXPECT_DOUBLE_EQ(sses[2], 0.0);
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelBlueChannelImageEqualSSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 0, 1};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto sses =
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 0.0);
  EXPECT_DOUBLE_EQ(sses[1], 0.0);
  EXPECT_DOUBLE_EQ(sses[2], 1.0);
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelRedAndBlueChannelImageEqualSSEOneInThoseChannels) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 1};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto sses =
      ImageMetrics::get_sum_of_squared_errors(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(sses[0], 1.0);
  EXPECT_DOUBLE_EQ(sses[1], 0.0);
  EXPECT_DOUBLE_EQ(sses[2], 1.0);
}


TEST_F(SmallBT601Image, RGBSameImageEqualZeroMSE) {
  for (const auto& mse : ImageMetrics::get_mse(bt601_image_a, bt601_image_a)) {
    EXPECT_DOUBLE_EQ(mse, 0.0);
  }
}


TEST_F(SmallBT601Image, RGBCopiedImageEqualMSEZero) {
  auto bt601_image_b = bt601_image_a;  //copy
  for (const auto& mse : ImageMetrics::get_mse(bt601_image_a, bt601_image_b)) {
    EXPECT_DOUBLE_EQ(mse, 0.0);
  }
}


TEST_F(SmallBT601Image, RGBChangedOnePixelImageEqualMSEOne) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 1, 1};
  bt601_image_b.set_pixel_at(new_color, 1, 1);

  for (const auto& mse : ImageMetrics::get_mse(bt601_image_a, bt601_image_b)) {
    EXPECT_DOUBLE_EQ(mse, 0.25);
  }
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelRedChannelImageEqualMSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 0};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.25);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelGreenChannelImageEqualMSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 1, 0};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.25);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelBlueChannelImageEqualMSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 0, 1};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.25);
}


TEST_F(SmallBT601Image,
    RGBChangedOnePixelRedAndBlueChannelImageEqualMSEOneInThoseChannels) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 1};
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.25);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.25);
}


TEST_F(SmallBT601Image, RGBChangedAllPixelsImageEqualMSEOne) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 1, 1};
  bt601_image_b.set_pixel_at(new_color, 0, 0);
  bt601_image_b.set_pixel_at(new_color, 0, 1);
  bt601_image_b.set_pixel_at(new_color, 1, 0);
  bt601_image_b.set_pixel_at(new_color, 1, 1);

  for (const auto& mse : ImageMetrics::get_mse(bt601_image_a, bt601_image_b)) {
    EXPECT_DOUBLE_EQ(mse, 1.0);
  }
}


TEST_F(SmallBT601Image,
    RGBChangedAllPixelsRedChannelImageEqualMSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 0};
  bt601_image_b.set_pixel_at(new_color, 0, 0);
  bt601_image_b.set_pixel_at(new_color, 0, 1);
  bt601_image_b.set_pixel_at(new_color, 1, 0);
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 1.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallBT601Image,
    RGBChangedAllPixelsGreenChannelImageEqualMSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 1, 0};
  bt601_image_b.set_pixel_at(new_color, 0, 0);
  bt601_image_b.set_pixel_at(new_color, 0, 1);
  bt601_image_b.set_pixel_at(new_color, 1, 0);
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 1.0);
  EXPECT_DOUBLE_EQ(mses[2], 0.0);
}


TEST_F(SmallBT601Image,
    RGBChangedAllPixelsBlueChannelImageEqualMSEOneInThatChannel) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {0, 0, 1};
  bt601_image_b.set_pixel_at(new_color, 0, 0);
  bt601_image_b.set_pixel_at(new_color, 0, 1);
  bt601_image_b.set_pixel_at(new_color, 1, 0);
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 0.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 1.0);
}


TEST_F(SmallBT601Image,
    RGBChangedAllPixelsRedAndBlueChannelImageEqualMSEOneInThoseChannels) {
  auto bt601_image_b = bt601_image_a;  //copy
  std::tuple<uint16_t, uint16_t, uint16_t> new_color = {1, 0, 1};
  bt601_image_b.set_pixel_at(new_color, 0, 0);
  bt601_image_b.set_pixel_at(new_color, 0, 1);
  bt601_image_b.set_pixel_at(new_color, 1, 0);
  bt601_image_b.set_pixel_at(new_color, 1, 1);
  auto mses = ImageMetrics::get_mse(bt601_image_a, bt601_image_b);
  EXPECT_DOUBLE_EQ(mses[0], 1.0);
  EXPECT_DOUBLE_EQ(mses[1], 0.0);
  EXPECT_DOUBLE_EQ(mses[2], 1.0);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}