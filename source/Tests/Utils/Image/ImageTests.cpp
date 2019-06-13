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

/** \file     ImageTests.cpp
 *  \brief    Tests of Image class.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-22
 */

#include <iostream>
#include "Lib/Utils/Image/RGBImage.h"
#include "Lib/Utils/Image/YCbCrImage.h"
#include "gtest/gtest.h"


struct GrayscaleImageTest : testing::Test {
  GrayScaleImage<uint8_t>* image;
  static constexpr auto width = 3;
  static constexpr auto height = 4;
  static constexpr auto bpp = 8;
  GrayscaleImageTest() {
    image = new GrayScaleImage<uint8_t>(width, height, bpp);
  }
  ~GrayscaleImageTest() {
    delete image;
  }
};


TEST_F(GrayscaleImageTest, GrayScaleImageHasOneChannel) {
  EXPECT_EQ(1, image->get_number_of_channels());
}


TEST_F(GrayscaleImageTest, GrayScaleImageHasRightNumberOfBits) {
  EXPECT_EQ(bpp, image->get_bpp());
}


TEST_F(GrayscaleImageTest, GrayScaleImageHasRightWidth) {
  EXPECT_EQ(width, image->get_width());
}


TEST_F(GrayscaleImageTest, GrayScaleImageHasRightHeight) {
  EXPECT_EQ(height, image->get_height());
}


TEST_F(GrayscaleImageTest, GrayScaleImageHasAGrayName) {
  EXPECT_EQ("Gray", image->get_channel_names()[0]);
}


TEST_F(GrayscaleImageTest, TheTypeOfGrayScaleImageIsGraysScale) {
  EXPECT_EQ(ImageType::GrayScale, image->get_type());
}


TEST_F(GrayscaleImageTest, GrayScaleHasRightNumberOfPixelsPerChannel) {
  EXPECT_EQ(width * height, image->get_number_of_pixels_per_channel());
}


TEST_F(GrayscaleImageTest, GrayScaleHasRightNumberOfPixels) {
  EXPECT_EQ(1 * width * height, image->get_number_of_pixels());
}


TEST_F(GrayscaleImageTest,
    GrayScaleEqualNumberOfPixelsAndNumberOfPixelsPerChannel) {
  EXPECT_EQ(
      image->get_number_of_pixels_per_channel(), image->get_number_of_pixels());
}


struct RGBImageTest : testing::Test {
  RGBImage<uint8_t>* image;
  static constexpr auto width = 3;
  static constexpr auto height = 4;
  static constexpr auto bpp = 8;
  RGBImageTest() {
    image = new RGBImage<uint8_t>(width, height, bpp);
  }
  ~RGBImageTest() {
    delete image;
  }
};


TEST_F(RGBImageTest, RGBImageHasThreeChannels) {
  EXPECT_EQ(3, image->get_number_of_channels());
}


TEST_F(RGBImageTest, RGBImageHasRightNumberOfBits) {
  EXPECT_EQ(bpp, image->get_bpp());
}


TEST_F(RGBImageTest, RGBImageHasRightWidth) {
  EXPECT_EQ(width, image->get_width());
}


TEST_F(RGBImageTest, RGBHasRightNumberOfPixelsPerChannel) {
  EXPECT_EQ(width * height, image->get_number_of_pixels_per_channel());
}


TEST_F(RGBImageTest, RGBHasRightNumberOfPixels) {
  EXPECT_EQ(3 * width * height, image->get_number_of_pixels());
}


TEST_F(RGBImageTest, RGBImageHasRightHeight) {
  EXPECT_EQ(height, image->get_height());
}


TEST_F(RGBImageTest, AllChannelsHaveTheSameDimensions) {
  auto [previous_width, previous_height] = std::make_pair(width, height);
  for (const auto& channel : image->get_channels()) {
    auto [current_width, current_height] = channel.get_dimension();
    EXPECT_EQ(previous_width, current_width);
    EXPECT_EQ(previous_height, current_height);
  }
}


TEST_F(RGBImageTest, AllChannelsHaveTheSameDimensionsByHasEqualSize) {
  auto R = image->get_channel(0);
  auto G = image->get_channel(1);
  auto B = image->get_channel(2);

  EXPECT_TRUE(R.has_equal_size(G));
  EXPECT_TRUE(R.has_equal_size(B));
}


TEST_F(RGBImageTest, TheTypeOfRGBImageIsRGB) {
  EXPECT_EQ(ImageType::RGB, image->get_type());
}


TEST_F(RGBImageTest, TheRGBNamesAreRedGreenBlue) {
  auto names = image->get_channel_names();
  EXPECT_EQ("Red", names[0]);
  EXPECT_EQ("Green", names[1]);
  EXPECT_EQ("Blue", names[2]);
}


TEST(RGBImageMove, AMoveAssignmentEmptiesTheImageContent) {
  auto image_to_move = RGBImage<uint8_t>(1, 1, 2);
  RGBImage<uint8_t> moved_image(1, 2, 3);
  moved_image = std::move(image_to_move);
  EXPECT_EQ(0, image_to_move.get_number_of_channels());
  EXPECT_EQ(3, moved_image.get_number_of_channels());
}


TEST(RGBImageMove, AMoveConstructorEmptiesTheImageContent) {
  auto image_to_move = RGBImage<uint8_t>(1, 1, 2);
  auto moved_image(std::move(image_to_move));
  EXPECT_EQ(0, image_to_move.get_number_of_channels());
}


TEST(
    RGBImageMove, AMoveConstructorCreatesAImageWithTheCorrectNumberOfChannels) {
  auto image_to_move = RGBImage<uint8_t>(1, 1, 2);
  auto moved_image(std::move(image_to_move));
  EXPECT_EQ(3, moved_image.get_number_of_channels());
}


TEST(RGBImageMove, AMoveConstructorCreatesAImageWithTheSameType) {
  auto image_to_move = RGBImage<uint8_t>(1, 1, 2);
  auto moved_image(std::move(image_to_move));
  EXPECT_EQ(ImageType::RGB, moved_image.get_type());
}


TEST(RGBImageMove, AMoveConstructorCreatesAImageThatUsesTheSameAlocatedMemory) {
  auto image_to_move = RGBImage<uint8_t>(1, 1, 2);
  auto red_channel_ptr = image_to_move.get_channel(0).data();
  auto green_channel_ptr = image_to_move.get_channel(1).data();
  auto blue_channel_ptr = image_to_move.get_channel(2).data();
  auto moved_image(std::move(image_to_move));
  EXPECT_EQ(red_channel_ptr, moved_image.get_channel(0).data());
  EXPECT_EQ(green_channel_ptr, moved_image.get_channel(1).data());
  EXPECT_EQ(blue_channel_ptr, moved_image.get_channel(2).data());
}


TEST(GrayScaleImageCopyConstruction, BothImagesHaveTheSameNumberOfChannels) {
  auto image_to_copy = GrayScaleImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy.get_number_of_channels(),
      copied_image.get_number_of_channels());
}


TEST(GrayScaleImageCopyConstruction,
    TheCopiedImageHaveNumberOfChannelsDifferentFromZero) {
  auto image_to_copy = GrayScaleImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_TRUE(copied_image.get_number_of_channels() != 0);
}


TEST(GrayScaleImageCopyConstruction, BothImagesHaveTheSameNumberOfPixels) {
  auto image_to_copy = GrayScaleImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy.get_number_of_pixels(),
      copied_image.get_number_of_pixels());
}


TEST(GrayScaleImageCopyConstruction, BothImagesHaveTheSamePixels) {
  auto image_to_copy = GrayScaleImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy.get_channel(0), copied_image.get_channel(0));
}


TEST(GrayScaleImageCopyConstruction, BothImagesAreEqual) {
  auto image_to_copy = GrayScaleImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy, copied_image);
}


TEST(RGBImageCopyConstruction, BothImagesHaveTheSameNumberOfChannels) {
  auto image_to_copy = RGBImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy.get_number_of_channels(),
      copied_image.get_number_of_channels());
}


TEST(RGBImageCopyConstruction,
    TheCopiedImageHaveNumberOfChannelsDifferentFromZero) {
  auto image_to_copy = RGBImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_TRUE(copied_image.get_number_of_channels() != 0);
}


TEST(RGBImageCopyConstruction, BothImagesHaveTheSameNumberOfPixels) {
  auto image_to_copy = RGBImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy.get_number_of_pixels(),
      copied_image.get_number_of_pixels());
}


TEST(RGBImageCopyConstruction, BothImagesHaveTheSamePixels) {
  auto image_to_copy = RGBImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy.get_channel(0), copied_image.get_channel(0));
  EXPECT_EQ(image_to_copy.get_channel(1), copied_image.get_channel(1));
  EXPECT_EQ(image_to_copy.get_channel(2), copied_image.get_channel(2));
}


TEST(RGBImageCopyConstruction, BothImagesAreEqual) {
  auto image_to_copy = RGBImage<uint8_t>(1, 1, 2);
  auto copied_image = image_to_copy;
  EXPECT_EQ(image_to_copy, copied_image);
}

TEST(GrayScaleImageMove,
    AMoveConstructorCreatesAImageThatUsesTheSameAlocatedMemory) {
  auto image_to_move = GrayScaleImage<uint8_t>(1, 1, 2);
  auto gray_channel_ptr = image_to_move.get_channel(0).data();
  auto moved_image(std::move(image_to_move));
  EXPECT_EQ(gray_channel_ptr, moved_image.get_channel(0).data());
}


TEST(GrayScaleImageMove, AMoveConstructorCreatesAImageWhoseChannelIsNotNull) {
  auto image_to_move = GrayScaleImage<uint8_t>(1, 1, 2);
  auto moved_image(std::move(image_to_move));
  auto gray_channel = moved_image.get_channel(0);
  EXPECT_TRUE(gray_channel.data() != nullptr);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}