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

/** \file     ImageChannelTests.cpp
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-20
 */

#include <iostream>
#include "ImageChannel.h"
#include "gtest/gtest.h"

struct ImageChannelTestUInt8T : testing::Test {
  ImageChannel<uint8_t>* image_channel;
  static constexpr auto width = 3;
  static constexpr auto height = 4;
  static constexpr auto bpp = 8;
  ImageChannelTestUInt8T() {
    image_channel = new ImageChannel<uint8_t>(width, height, bpp);
  }
  ~ImageChannelTestUInt8T() {
    delete image_channel;
  }
};


TEST_F(ImageChannelTestUInt8T, FillWithZeroSetsAllPixelsToZero) {
  image_channel->fill_with(0);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(0, pixel);
  }
}


TEST_F(ImageChannelTestUInt8T, FillWithOneSetsAllPixelsToOne) {
  image_channel->fill_with(1);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(1, pixel);
  }
}


TEST_F(ImageChannelTestUInt8T, FillWithMaxValueSetsAllPixelsToMaxValue) {
  auto max_value = std::numeric_limits<uint8_t>::max();
  image_channel->fill_with(max_value);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(max_value, pixel);
  }
}


struct ImageChannelTestIntT : testing::Test {
  ImageChannel<int>* image_channel;
  static constexpr auto width = 3;
  static constexpr auto height = 4;
  static constexpr auto bpp = 16;
  ImageChannelTestIntT() {
    image_channel = new ImageChannel<int>(width, height, bpp);
  }
  ~ImageChannelTestIntT() {
    delete image_channel;
  }
};


TEST_F(ImageChannelTestIntT, FillWithZeroSetsAllPixelsToZero) {
  image_channel->fill_with(0);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(0, pixel);
  }
}


TEST_F(ImageChannelTestIntT, FillWithOneSetsAllPixelsToOne) {
  image_channel->fill_with(1);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(1, pixel);
  }
}


TEST_F(ImageChannelTestIntT, FillWithMaxValueSetsAllPixelsToMaxValue) {
  auto max_value = std::numeric_limits<int>::max();
  image_channel->fill_with(max_value);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(max_value, pixel);
  }
}


TEST_F(ImageChannelTestIntT, MaxValueOfSignedType) {
  EXPECT_EQ(static_cast<int>(std::pow(2, bpp - 1) - 1),
      image_channel->get_max_value());
}


TEST_F(ImageChannelTestIntT, MinValueOfSignedType) {
  EXPECT_EQ(-1 * static_cast<int>(std::pow(2, bpp - 1)),
      image_channel->get_min_value());
}


struct ImageChannelTestUInt16T : testing::Test {
  ImageChannel<uint16_t>* image_channel;
  static constexpr auto width = 3;
  static constexpr auto height = 4;
  static constexpr auto bpp = 16;
  ImageChannelTestUInt16T() {
    image_channel = new ImageChannel<uint16_t>(width, height, bpp);
  }
  ~ImageChannelTestUInt16T() {
    delete image_channel;
  }
};


TEST_F(ImageChannelTestUInt16T, MaxValueOfUnsignedType) {
  EXPECT_EQ(
      static_cast<int>(std::pow(2, bpp) - 1), image_channel->get_max_value());
}


TEST_F(ImageChannelTestUInt16T, MinValueOfUnsignedType) {
  EXPECT_EQ(0, image_channel->get_min_value());
}


TEST_F(ImageChannelTestUInt16T, ImageChannelGetWidth) {
  EXPECT_EQ(width, image_channel->get_width());
}


TEST_F(ImageChannelTestUInt16T, ImageChannelGetHeight) {
  EXPECT_EQ(height, image_channel->get_height());
}


TEST_F(ImageChannelTestUInt16T, ImageChannelGetDimensionGetsPairWidthHeight) {
  auto [w, h] = image_channel->get_dimension();
  EXPECT_EQ(width, w);
  EXPECT_EQ(height, h);
}


TEST_F(ImageChannelTestUInt16T, ImageChannelGetBpp) {
  EXPECT_EQ(bpp, image_channel->get_bpp());
}


TEST_F(ImageChannelTestUInt16T, ImageChannelGetNumberOfPixels) {
  EXPECT_EQ(width * height, image_channel->get_number_of_pixels());
}


TEST_F(ImageChannelTestUInt16T, ValidIndexMustReturnTheSameWhenUsingPairOrNot) {
  for (auto i = 0; i < height + 100; ++i) {
    for (auto j = 0; j < width + 100; ++j) {
      EXPECT_EQ(image_channel->is_coordinate_valid({i, j}),
          image_channel->is_coordinate_valid(i, j));
    }
  }
}


TEST_F(ImageChannelTestUInt16T, ValidIndexMustBeValidated) {
  for (auto i = 0; i < height; ++i) {
    for (auto j = 0; j < width; ++j) {
      EXPECT_TRUE(image_channel->is_coordinate_valid(i, j));
    }
  }
}


TEST_F(ImageChannelTestUInt16T, InvalidIndexMustBeNotValidated) {
  EXPECT_FALSE(image_channel->is_coordinate_valid(width - 1, height));
  EXPECT_FALSE(image_channel->is_coordinate_valid(width, height - 1));
  EXPECT_FALSE(image_channel->is_coordinate_valid(width, height));
}


TEST_F(ImageChannelTestUInt16T, SetInInvalidIndexMustThrow) {
  auto value = 42;
  std::cout << value << std::endl;
  EXPECT_THROW(image_channel->set_value_at(value, width - 1, height),
      ImageChannelExceptions::InvalidIndexWriteException);
  EXPECT_THROW(image_channel->set_value_at(value, width, height - 1),
      ImageChannelExceptions::InvalidIndexWriteException);
  EXPECT_THROW(image_channel->set_value_at(value, width, height),
      ImageChannelExceptions::InvalidIndexWriteException);
}


TEST_F(ImageChannelTestUInt16T, GetInInvalidIndexMustThrow) {
  EXPECT_THROW(image_channel->get_value_at(width - 1, height),
      ImageChannelExceptions::InvalidIndexReadException);
  EXPECT_THROW(image_channel->get_value_at(width, height - 1),
      ImageChannelExceptions::InvalidIndexReadException);
  EXPECT_THROW(image_channel->get_value_at(width, height),
      ImageChannelExceptions::InvalidIndexReadException);
}


TEST_F(ImageChannelTestUInt16T,
    AllValuesAreValidIfBppHasTheSameNumberOfBitsThanItsContainerType) {
  for (uint16_t i = 0; i < std::numeric_limits<uint16_t>::max(); ++i) {
    EXPECT_NO_THROW(image_channel->set_value_at(i, 0, 0));
  }
  EXPECT_NO_THROW(
      image_channel->set_value_at(std::numeric_limits<uint16_t>::max(), 0, 0));
}


TEST_F(ImageChannelTestUInt16T, FillWithZeroSetsAllPixelsToZero) {
  image_channel->fill_with(0);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(0, pixel);
  }
}


TEST_F(ImageChannelTestUInt16T, FillWithOneSetsAllPixelsToOne) {
  image_channel->fill_with(1);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(1, pixel);
  }
}


TEST_F(ImageChannelTestUInt16T, FillWithMaxValueSetsAllPixelsToMaxValue) {
  auto max_value = std::numeric_limits<uint16_t>::max();
  image_channel->fill_with(max_value);
  for (const auto& pixel : image_channel->as_raster_vector()) {
    EXPECT_EQ(max_value, pixel);
  }
}


TEST_F(ImageChannelTestUInt16T, APixelSetCanBeRetrivedByGet) {
  uint16_t pixel_value = 42;
  image_channel->set_value_at(pixel_value, 0, 0);
  EXPECT_EQ(pixel_value, image_channel->get_value_at(0, 0));
  pixel_value = 128;
  image_channel->set_value_at(pixel_value, 1, 0);
  EXPECT_EQ(pixel_value, image_channel->get_value_at(1, 0));
}


TEST_F(ImageChannelTestUInt16T, GetIsEquivalentTo2DAccess) {
  uint16_t pixel_value = 42;
  image_channel->set_value_at(pixel_value, 0, 0);
  EXPECT_EQ(image_channel->get_value_at(0, 0), (*image_channel)[0][0]);

  image_channel->set_value_at(51, 2, 0);
  EXPECT_EQ(image_channel->get_value_at(2, 0), (*image_channel)[2][0]);
}


TEST_F(ImageChannelTestUInt16T, APixelSetWillOverwritePeviousSet) {
  uint16_t pixel_value_previous = 4;
  image_channel->set_value_at(pixel_value_previous, 0, 0);
  EXPECT_EQ(pixel_value_previous, image_channel->get_value_at(0, 0));
  uint16_t pixel_value = 42;
  image_channel->set_value_at(pixel_value, 0, 0);
  EXPECT_EQ(pixel_value, image_channel->get_value_at(0, 0));
}


TEST_F(ImageChannelTestUInt16T, SameSizeInImagesCreatedWithSameSize) {
  ImageChannel<uint16_t> other(width, height, bpp);
  EXPECT_TRUE(image_channel->has_equal_size(other));
}

TEST_F(ImageChannelTestUInt16T, AChannelHasSameSizeAsItself) {
  EXPECT_TRUE(image_channel->has_equal_size(*image_channel));
}

TEST_F(ImageChannelTestUInt16T, DifferentSizeInImagesCreatedWithDifferentSize) {
  ImageChannel<uint16_t> other(width + 1, height + 2, bpp);
  EXPECT_FALSE(image_channel->has_equal_size(other));
}


TEST_F(ImageChannelTestUInt16T, DifferenceOfTwoChannelsHasSameSize) {
  ImageChannel<uint16_t> other(width, height, bpp);
  auto difference = (*image_channel) - other;
  EXPECT_TRUE(image_channel->has_equal_size(other));
  EXPECT_EQ(width, difference.get_width());
  EXPECT_EQ(height, difference.get_height());
}


TEST_F(ImageChannelTestUInt16T, DifferenceOfTwoChannelsHasOneMoreBpp) {
  ImageChannel<uint16_t> other(width, height, bpp);
  auto difference = (*image_channel) - other;
  EXPECT_EQ(bpp + 1, difference.get_bpp());
}


TEST_F(ImageChannelTestUInt16T, DifferenceOfTwoZeroImagesIsZero) {
  ImageChannel<uint16_t> other(width, height, bpp);
  image_channel->fill_with(0);
  other.fill_with(0);
  auto difference = (*image_channel) - other;
  for (const auto& pixel : difference.as_raster_vector()) {
    EXPECT_EQ(0, pixel);
  }
}


TEST_F(ImageChannelTestUInt16T, DifferenceOfTwoChannelsIsNotComutative) {
  ImageChannel<uint16_t> other(width, height, bpp);
  image_channel->fill_with(0);
  other.fill_with(42);
  auto difference_a = (*image_channel) - other;
  auto difference_b = other - (*image_channel);
  EXPECT_FALSE(difference_a == difference_b);
}


TEST_F(ImageChannelTestUInt16T,
    DifferenceOfTwoChannelsIsNegativeIfSecondTermIsLargerThanFirst) {
  ImageChannel<uint16_t> other(width, height, bpp);
  image_channel->fill_with(0);
  uint16_t larger_value = 42;
  other.fill_with(larger_value);
  auto difference_a = (*image_channel) - other;
  ImageChannel<int16_t> other_negative(width, height, bpp);
  other_negative.fill_with(-larger_value);
  EXPECT_TRUE(difference_a == other_negative);
}


TEST(InvalidImageChannel, ChannelCannotHaveWidthEqualsToZero) {
  EXPECT_THROW(ImageChannel<uint16_t> image_channel(0, 1, 1),
      ImageChannelExceptions::InvalidSizeException);
}


TEST(InvalidImageChannel, ChannelCannotHaveHeightEqualsToZero) {
  EXPECT_THROW(ImageChannel<uint16_t> image_channel(1, 0, 1),
      ImageChannelExceptions::InvalidSizeException);
}


TEST(InvalidImageChannel, ChannelCannotHaveBppEqualsToZero) {
  EXPECT_THROW(ImageChannel<uint16_t> image_channel(1, 1, 0),
      ImageChannelExceptions::InvalidSizeException);
}


TEST(InvalidValueSet, AValueCannotExceedTheLimitImposedByBpp) {
  auto bpp = 10;
  ImageChannel<uint16_t> image_channel(1, 1, bpp);
  EXPECT_THROW(image_channel.set_value_at(1024, {0, 0}),
      ImageChannelExceptions::InvalidValueException);
}


TEST(InvalidValueSet, AValueCannotExceedTheLimitImposedByBppEvenIfNegative) {
  auto bpp = 11;
  ImageChannel<int16_t> image_channel(1, 1, bpp);
  EXPECT_THROW(image_channel.set_value_at(1024, {0, 0}),
      ImageChannelExceptions::InvalidValueException);
  EXPECT_THROW(image_channel.set_value_at(-1025, {0, 0}),
      ImageChannelExceptions::InvalidValueException);
}


TEST(InvalidValueSet, AValueIsValidInTheIntervalDefinedByTheBpp) {
  auto bpp = 11;
  ImageChannel<int16_t> image_channel(1, 1, bpp);
  for (int16_t i = -1024; i < 1024; ++i) {
    EXPECT_NO_THROW(image_channel.set_value_at(i, {0, 0}));
  }
}


TEST(InvalidValueSet, NoThrowIfBelowMaxDefinedByBppThrowsOtherwise) {
  auto bpp = 10;
  ImageChannel<uint16_t> image_channel(1, 1, bpp);
  for (uint16_t i = 0; i < 1024; ++i) {
    EXPECT_NO_THROW(image_channel.set_value_at(i, {0, 0}));
  }
  for (uint16_t i = 1024; i < std::numeric_limits<uint16_t>::max(); ++i) {
    EXPECT_THROW(image_channel.set_value_at(i, {0, 0}),
        ImageChannelExceptions::InvalidValueException);
  }
  EXPECT_THROW(
      image_channel.set_value_at(std::numeric_limits<uint16_t>::max(), {0, 0}),
      ImageChannelExceptions::InvalidValueException);
}


struct ImageChannelMove : testing::Test {
  static constexpr auto width = 2;
  static constexpr auto height = 3;
  static constexpr auto bpp = 10;
};


TEST_F(ImageChannelMove, AMoveAssignmentEmptiesContent) {
  ImageChannel<uint16_t> channel_to_be_moved(width, height, bpp);
  ImageChannel<uint16_t> moved_channel(width, height, bpp);
  moved_channel = std::move(channel_to_be_moved);
  EXPECT_EQ(nullptr, channel_to_be_moved.data());
}


TEST_F(ImageChannelMove, AMoveConstructorEmptiesContent) {
  ImageChannel<uint16_t> channel_to_be_moved(width, height, bpp);
  auto moved_channel = std::move(channel_to_be_moved);
  EXPECT_EQ(nullptr, channel_to_be_moved.data());
}


TEST_F(ImageChannelMove, AMoveConstructorCreatesAChannelWithTheCorrectBpp) {
  ImageChannel<uint16_t> channel_to_be_moved(width, height, bpp);
  auto moved_channel = std::move(channel_to_be_moved);
  EXPECT_EQ(bpp, moved_channel.get_bpp());
}


TEST_F(ImageChannelMove, AMoveConstructorCreatesAChannelWithTheCorrectWidth) {
  ImageChannel<uint16_t> channel_to_be_moved(width, height, bpp);
  auto moved_channel = std::move(channel_to_be_moved);
  EXPECT_EQ(width, moved_channel.get_width());
}


TEST_F(ImageChannelMove, AMoveConstructorCreatesAChannelWithTheCorrectHeight) {
  ImageChannel<uint16_t> channel_to_be_moved(width, height, bpp);
  auto moved_channel = std::move(channel_to_be_moved);
  EXPECT_EQ(height, moved_channel.get_height());
}


TEST_F(ImageChannelMove,
    AMoveConstructorCreatesAChannelWithTheCorrectNumberOfPixels) {
  ImageChannel<uint16_t> channel_to_be_moved(width, height, bpp);
  auto moved_channel = std::move(channel_to_be_moved);
  EXPECT_EQ(width * height, moved_channel.get_number_of_pixels());
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
