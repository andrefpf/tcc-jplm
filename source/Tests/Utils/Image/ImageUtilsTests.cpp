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

/** \file     ImageUtilsTests.cpp
 *  \brief    Test of lightfield.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-13
 *
 */

#include <iostream>
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageUtils.h"
#include "gtest/gtest.h"


struct ImageContainerCheck : public testing::Test {
  std::size_t width = 4;
  std::size_t height = 3;
  std::unique_ptr<RGBImage<uint16_t>> image_unsigned;
  std::unique_ptr<RGBImage<int16_t>> image_signed;
  ImageContainerCheck()
      : image_unsigned(std::make_unique<RGBImage<uint16_t>>(width, height, 10)),
        image_signed(std::make_unique<RGBImage<int16_t>>(width, height, 10)) {
    for (auto& channel : *image_unsigned) {
      for (auto& value : channel) {
        value = 0;
      }
    }
    for (auto& channel : *image_signed) {
      for (auto& value : channel) {
        value = 0;
      }
    }
  }
};


TEST_F(ImageContainerCheck, Uint16FitsInUint16) {
  EXPECT_NO_THROW(
      ImageUtils::check_image_container_requisites<uint16_t>(*image_unsigned));
}


TEST_F(ImageContainerCheck, 10bppDoNotFitInUint8ThusThrows) {
  EXPECT_THROW(
      ImageUtils::check_image_container_requisites<uint8_t>(*image_unsigned),
      ImageUtilsExceptions::ContainerHasFewerBitsThanNeededException);
}


TEST_F(ImageContainerCheck, UnsignedValuesCanBeIncludedInUintThus) {
  EXPECT_NO_THROW(
      ImageUtils::check_image_container_requisites<uint16_t>(*image_signed));
}


TEST_F(ImageContainerCheck, NegativeValuesCannotBeIncludedInUintThusThrows) {
  image_signed->set_pixel_at({-1, 0, 0}, 0, 0);
  EXPECT_THROW(
      ImageUtils::check_image_container_requisites<uint16_t>(*image_signed),
      ImageUtilsExceptions::NegativeValueInUnsignedVariableException);
}


struct MergingOfUndefinedImages : public testing::Test {
  std::size_t width = 2;
  std::size_t height = 3;
  std::size_t bpp = 10;

  std::unique_ptr<UndefinedImage<uint16_t>> channel_0 =
      std::make_unique<UndefinedImage<uint16_t>>(width, height, bpp, 1);
  std::unique_ptr<UndefinedImage<uint16_t>> channel_1 =
      std::make_unique<UndefinedImage<uint16_t>>(width, height, bpp, 1);
  std::unique_ptr<UndefinedImage<uint16_t>> channel_2 =
      std::make_unique<UndefinedImage<uint16_t>>(width, height, bpp, 1);

  MergingOfUndefinedImages() {
    uint16_t pel = 0;
    for (auto i = decltype(height){0}; i < height; ++i) {
      for (auto j = decltype(width){0}; j < width; ++j) {
        channel_0->set_pixel_at(
            pel++, 0, i, j);
        channel_1->set_pixel_at(
            pel++, 0, i, j);
        channel_2->set_pixel_at(
            pel++, 0, i, j);
      }
    }
  }
};


TEST_F(MergingOfUndefinedImages, MergingDoesNotThrow) {
  EXPECT_NO_THROW(auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2));
}


TEST_F(MergingOfUndefinedImages, MergingResultsInThreeChannels) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  EXPECT_EQ(merged->get_number_of_channels(), 3);
}


TEST_F(MergingOfUndefinedImages, MergingResultsOnTheSameWidth) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  EXPECT_EQ(merged->get_width(), channel_0->get_width());
}


TEST_F(MergingOfUndefinedImages, MergingResultsOnTheSameHeight) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  EXPECT_EQ(merged->get_height(), channel_0->get_height());
}


TEST_F(MergingOfUndefinedImages, MergingResultsOnTheSameBpp) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  EXPECT_EQ(merged->get_bpp(), channel_0->get_bpp());
}


TEST_F(MergingOfUndefinedImages, MergingResultsOnTheExpectedType) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  EXPECT_EQ(merged->get_type(), ImageType::BT601);
}


TEST_F(MergingOfUndefinedImages, MergingResultsOnTheCorrectValues) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  for (auto i = decltype(height){0}; i < height; ++i) {
    for (auto j = decltype(width){0}; j < width; ++j) {
      EXPECT_EQ(merged->get_value_at(0, i, j), channel_0->get_value_at(0, i, j));
      EXPECT_EQ(merged->get_value_at(1, i, j), channel_1->get_value_at(0, i, j));
      EXPECT_EQ(merged->get_value_at(2, i, j), channel_2->get_value_at(0, i, j));
    }
  }
}


TEST_F(MergingOfUndefinedImages, SplittigResultsOnTheExpectedType) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  auto split = ImageUtils::get_splitting_of(*merged);

  for(const auto& image: split) {
    EXPECT_EQ(image->get_type(), ImageType::Undefined);
  }
}


TEST_F(MergingOfUndefinedImages, SplittigResultsOnTheExpectedWidth) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  auto split = ImageUtils::get_splitting_of(*merged);

  for(const auto& image: split) {
    EXPECT_EQ(image->get_width(), width);
  }
}


TEST_F(MergingOfUndefinedImages, SplittigResultsOnTheExpectedHeight) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  auto split = ImageUtils::get_splitting_of(*merged);

  for(const auto& image: split) {
    EXPECT_EQ(image->get_height(), height);
  }
}


TEST_F(MergingOfUndefinedImages, SplittigResultsOnTheExpectedBpp) {
  auto merged = ImageUtils::get_undefined_images_as<BT601Image>(*channel_0, *channel_1, *channel_2);
  auto split = ImageUtils::get_splitting_of(*merged);

  for(const auto& image: split) {
    EXPECT_EQ(image->get_bpp(), bpp);
  }
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
