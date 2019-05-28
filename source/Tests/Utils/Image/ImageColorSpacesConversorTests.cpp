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
 *  \date     2019-05-28
 */

#include "Image.h"
#include "ImageColorSpacesConversor.h"
#include "gtest/gtest.h"

struct SimpleImageTypeChecksFromRGB : testing::Test {
 protected:
  RGBImage<uint16_t>* rgb_image;
  SimpleImageTypeChecksFromRGB() {
    rgb_image = new RGBImage<uint16_t>(2, 3, 8);
  }
};

TEST_F(SimpleImageTypeChecksFromRGB, RGBToRGBResultsInRGB) {
  auto rgb_image_2 =
      ImageColorSpaceConversion::convert::to<RGBImage>(*rgb_image);
  EXPECT_EQ(rgb_image_2.get_type(), ImageType::RGB);
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToRGBResultsInRGBWithSameWidth) {
  auto rgb_image_2 =
      ImageColorSpaceConversion::convert::to<RGBImage>(*rgb_image);
  EXPECT_EQ(rgb_image_2.get_width(), rgb_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToRGBResultsInRGBWithSameHeight) {
  auto rgb_image_2 =
      ImageColorSpaceConversion::convert::to<RGBImage>(*rgb_image);
  EXPECT_EQ(rgb_image_2.get_height(), rgb_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToRGBResultsInRGBWithSameBpp) {
  auto rgb_image_2 =
      ImageColorSpaceConversion::convert::to<RGBImage>(*rgb_image);
  EXPECT_EQ(rgb_image_2.get_bpp(), rgb_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT601ResultsInBT601) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*rgb_image);
  EXPECT_EQ(bt_601_image.get_type(), ImageType::BT601);
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT601ResultsInBT601WithSameWidth) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*rgb_image);
  EXPECT_EQ(bt_601_image.get_width(), rgb_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT601ResultsInBT601WithSameHeight) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*rgb_image);
  EXPECT_EQ(bt_601_image.get_height(), rgb_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT601ResultsInBT601WithSameBpp) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*rgb_image);
  EXPECT_EQ(bt_601_image.get_bpp(), rgb_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT709ResultsInBT709) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*rgb_image);
  EXPECT_EQ(bt_709_image.get_type(), ImageType::BT709);
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT709ResultsInBT709WithSameWidth) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*rgb_image);
  EXPECT_EQ(bt_709_image.get_width(), rgb_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT709ResultsInBT709WithSameHeight) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*rgb_image);
  EXPECT_EQ(bt_709_image.get_height(), rgb_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT709ResultsInBT709WithSameBpp) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*rgb_image);
  EXPECT_EQ(bt_709_image.get_bpp(), rgb_image->get_bpp());
}

TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT2020ResultsInBT2020) {
  auto bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*rgb_image);
  EXPECT_EQ(bt_2020_image.get_type(), ImageType::BT2020);
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT2020ResultsInBT2020WithSameWidth) {
  auto bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*rgb_image);
  EXPECT_EQ(bt_2020_image.get_width(), rgb_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT2020ResultsInBT2020WithSameHeight) {
  auto bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*rgb_image);
  EXPECT_EQ(bt_2020_image.get_height(), rgb_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromRGB, RGBToBT2020ResultsInBT2020WithSameBpp) {
  auto bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*rgb_image);
  EXPECT_EQ(bt_2020_image.get_bpp(), rgb_image->get_bpp());
}


struct SimpleImageTypeChecksFromBT2020 : testing::Test {
 protected:
  BT2020Image<uint16_t>* bt_2020_image;
  SimpleImageTypeChecksFromBT2020() {
    bt_2020_image = new BT2020Image<uint16_t>(2, 3, 8);
  }
};


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToBT2020ResultsInBT2020) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_2020_image);
  EXPECT_EQ(converted_bt_2020_image.get_type(), ImageType::BT2020);
}


TEST_F(SimpleImageTypeChecksFromBT2020,
    BT2020ToBT2020ResultsInBT2020WithSameWidth) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_2020_image);
  EXPECT_EQ(converted_bt_2020_image.get_width(), bt_2020_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT2020,
    BT2020ToBT2020ResultsInBT2020WithSameHeight) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_2020_image);
  EXPECT_EQ(converted_bt_2020_image.get_height(), bt_2020_image->get_height());
}


TEST_F(
    SimpleImageTypeChecksFromBT2020, BT2020ToBT2020ResultsInBT2020WithSameBpp) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_2020_image);
  EXPECT_EQ(converted_bt_2020_image.get_bpp(), bt_2020_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToRGBResultsInRGB) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_2020_image);
  EXPECT_EQ(rgb_image.get_type(), ImageType::RGB);
}


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToRGBResultsInRGBWithSameWidth) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_2020_image);
  EXPECT_EQ(rgb_image.get_width(), bt_2020_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToRGBResultsInRGBWithSameHeight) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_2020_image);
  EXPECT_EQ(rgb_image.get_height(), bt_2020_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToRGBResultsInRGBWithSameBpp) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_2020_image);
  EXPECT_EQ(rgb_image.get_bpp(), bt_2020_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToBT601ResultsInBT601) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_2020_image);
  EXPECT_EQ(bt_601_image.get_type(), ImageType::BT601);
}


TEST_F(
    SimpleImageTypeChecksFromBT2020, BT2020ToBT601ResultsInBT601WithSameWidth) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_2020_image);
  ASSERT_EQ(bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(bt_601_image.get_width(), bt_2020_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT2020,
    BT2020ToBT601ResultsInBT601WithSameHeight) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_2020_image);
  ASSERT_EQ(bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(bt_601_image.get_height(), bt_2020_image->get_height());
}


TEST_F(
    SimpleImageTypeChecksFromBT2020, BT2020ToBT601ResultsInBT601WithSameBpp) {
  auto bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_2020_image);
  ASSERT_EQ(bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(bt_601_image.get_bpp(), bt_2020_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT2020, BT2020ToBT709ResultsInBT709) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_2020_image);
  EXPECT_EQ(bt_709_image.get_type(), ImageType::BT709);
}


TEST_F(
    SimpleImageTypeChecksFromBT2020, BT2020ToBT709ResultsInBT709WithSameWidth) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_2020_image);
  ASSERT_EQ(bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(bt_709_image.get_width(), bt_2020_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT2020,
    BT2020ToBT709ResultsInBT709WithSameHeight) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_2020_image);
  ASSERT_EQ(bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(bt_709_image.get_height(), bt_2020_image->get_height());
}


TEST_F(
    SimpleImageTypeChecksFromBT2020, BT2020ToBT709ResultsInBT709WithSameBpp) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_2020_image);
  ASSERT_EQ(bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(bt_709_image.get_bpp(), bt_2020_image->get_bpp());
}


struct SimpleImageTypeChecksFromBT601 : testing::Test {
 protected:
  BT601Image<uint16_t>* bt_601_image;
  SimpleImageTypeChecksFromBT601() {
    bt_601_image = new BT601Image<uint16_t>(2, 3, 8);
  }
};


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToBT2020ResultsInBT2020) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_601_image);
  EXPECT_EQ(converted_bt_2020_image.get_type(), ImageType::BT2020);
}


TEST_F(
    SimpleImageTypeChecksFromBT601, BT601ToBT2020ResultsInBT2020WithSameWidth) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_601_image);
  EXPECT_EQ(converted_bt_2020_image.get_width(), bt_601_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT601,
    BT601ToBT2020ResultsInBT2020WithSameHeight) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_601_image);
  EXPECT_EQ(converted_bt_2020_image.get_height(), bt_601_image->get_height());
}


TEST_F(
    SimpleImageTypeChecksFromBT601, BT601ToBT2020ResultsInBT2020WithSameBpp) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_601_image);
  EXPECT_EQ(converted_bt_2020_image.get_bpp(), bt_601_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToRGBResultsInRGB) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_601_image);
  EXPECT_EQ(rgb_image.get_type(), ImageType::RGB);
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToRGBResultsInRGBWithSameWidth) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_601_image);
  EXPECT_EQ(rgb_image.get_width(), bt_601_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToRGBResultsInRGBWithSameHeight) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_601_image);
  EXPECT_EQ(rgb_image.get_height(), bt_601_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToRGBResultsInRGBWithSameBpp) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_601_image);
  EXPECT_EQ(rgb_image.get_bpp(), bt_601_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToBT601ResultsInBT601) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_601_image);
  EXPECT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
}


TEST_F(
    SimpleImageTypeChecksFromBT601, BT601ToBT601ResultsInBT601WithSameWidth) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_601_image);
  ASSERT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(converted_bt_601_image.get_width(), bt_601_image->get_width());
}


TEST_F(
    SimpleImageTypeChecksFromBT601, BT601ToBT601ResultsInBT601WithSameHeight) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_601_image);
  ASSERT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(converted_bt_601_image.get_height(), bt_601_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToBT601ResultsInBT601WithSameBpp) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_601_image);
  ASSERT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(converted_bt_601_image.get_bpp(), bt_601_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToBT709ResultsInBT709) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_601_image);
  EXPECT_EQ(bt_709_image.get_type(), ImageType::BT709);
}


TEST_F(
    SimpleImageTypeChecksFromBT601, BT601ToBT709ResultsInBT709WithSameWidth) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_601_image);
  ASSERT_EQ(bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(bt_709_image.get_width(), bt_601_image->get_width());
}


TEST_F(
    SimpleImageTypeChecksFromBT601, BT601ToBT709ResultsInBT709WithSameHeight) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_601_image);
  ASSERT_EQ(bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(bt_709_image.get_height(), bt_601_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT601, BT601ToBT709ResultsInBT709WithSameBpp) {
  auto bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_601_image);
  ASSERT_EQ(bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(bt_709_image.get_bpp(), bt_601_image->get_bpp());
}


struct SimpleImageTypeChecksFromBT709 : testing::Test {
 protected:
  BT709Image<uint16_t>* bt_709_image;
  SimpleImageTypeChecksFromBT709() {
    bt_709_image = new BT709Image<uint16_t>(2, 3, 8);
  }
};


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToBT2020ResultsInBT2020) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_709_image);
  EXPECT_EQ(converted_bt_2020_image.get_type(), ImageType::BT2020);
}


TEST_F(
    SimpleImageTypeChecksFromBT709, BT709ToBT2020ResultsInBT2020WithSameWidth) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_709_image);
  EXPECT_EQ(converted_bt_2020_image.get_width(), bt_709_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT709,
    BT709ToBT2020ResultsInBT2020WithSameHeight) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_709_image);
  EXPECT_EQ(converted_bt_2020_image.get_height(), bt_709_image->get_height());
}


TEST_F(
    SimpleImageTypeChecksFromBT709, BT709ToBT2020ResultsInBT2020WithSameBpp) {
  auto converted_bt_2020_image =
      ImageColorSpaceConversion::convert::to<BT2020Image>(*bt_709_image);
  EXPECT_EQ(converted_bt_2020_image.get_bpp(), bt_709_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToRGBResultsInRGB) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_709_image);
  EXPECT_EQ(rgb_image.get_type(), ImageType::RGB);
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToRGBResultsInRGBWithSameWidth) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_709_image);
  EXPECT_EQ(rgb_image.get_width(), bt_709_image->get_width());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToRGBResultsInRGBWithSameHeight) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_709_image);
  EXPECT_EQ(rgb_image.get_height(), bt_709_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToRGBResultsInRGBWithSameBpp) {
  auto rgb_image =
      ImageColorSpaceConversion::convert::to<RGBImage>(*bt_709_image);
  EXPECT_EQ(rgb_image.get_bpp(), bt_709_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToBT601ResultsInBT601) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_709_image);
  EXPECT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
}


TEST_F(
    SimpleImageTypeChecksFromBT709, BT709ToBT601ResultsInBT601WithSameWidth) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_709_image);
  ASSERT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(converted_bt_601_image.get_width(), bt_709_image->get_width());
}


TEST_F(
    SimpleImageTypeChecksFromBT709, BT709ToBT601ResultsInBT601WithSameHeight) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_709_image);
  ASSERT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(converted_bt_601_image.get_height(), bt_709_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToBT601ResultsInBT601WithSameBpp) {
  auto converted_bt_601_image =
      ImageColorSpaceConversion::convert::to<BT601Image>(*bt_709_image);
  ASSERT_EQ(converted_bt_601_image.get_type(), ImageType::BT601);
  EXPECT_EQ(converted_bt_601_image.get_bpp(), bt_709_image->get_bpp());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToBT709ResultsInBT709) {
  auto converted_bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_709_image);
  EXPECT_EQ(converted_bt_709_image.get_type(), ImageType::BT709);
}


TEST_F(
    SimpleImageTypeChecksFromBT709, BT709ToBT709ResultsInBT709WithSameWidth) {
  auto converted_bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_709_image);
  ASSERT_EQ(converted_bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(converted_bt_709_image.get_width(), bt_709_image->get_width());
}


TEST_F(
    SimpleImageTypeChecksFromBT709, BT709ToBT709ResultsInBT709WithSameHeight) {
  auto converted_bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_709_image);
  ASSERT_EQ(converted_bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(converted_bt_709_image.get_height(), bt_709_image->get_height());
}


TEST_F(SimpleImageTypeChecksFromBT709, BT709ToBT709ResultsInBT709WithSameBpp) {
  auto converted_bt_709_image =
      ImageColorSpaceConversion::convert::to<BT709Image>(*bt_709_image);
  ASSERT_EQ(converted_bt_709_image.get_type(), ImageType::BT709);
  EXPECT_EQ(converted_bt_709_image.get_bpp(), bt_709_image->get_bpp());
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}