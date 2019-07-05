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

/** \file     ImageIOTests.cpp
 *  \brief    Test Image File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-05
 */

#include <iostream>
#include <string>
#include "Lib/Utils/Image/ImageIO.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(ImReadTests, ReadBT601ImageCorrectType) {
  auto image = ImageIO::imread<BT601Image, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_type(), ImageType::BT601);
}


TEST(ImReadTests, ReadBT709ImageCorrectType) {
  auto image = ImageIO::imread<BT709Image, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_type(), ImageType::BT709);
}


TEST(ImReadTests, ReadBT2020ImageCorrectType) {
  auto image = ImageIO::imread<BT2020Image, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_type(), ImageType::BT2020);
}


TEST(ImReadTests, ReadYCbCrImageCorrectWidth) {
  auto image = ImageIO::imread<BT601Image, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_width(), 13);
}


TEST(ImReadTests, ReadYCbCrImageCorrectHeight) {
  auto image = ImageIO::imread<BT601Image, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_height(), 13);
}


TEST(ImReadTests, ReadRGBImageCorrectType) {
  auto image = ImageIO::imread<RGBImage, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_type(), ImageType::RGB);
}


TEST(ImReadTests, ReadRGBImageCorrectValueInTheMiddle) {
  auto image = ImageIO::imread<RGBImage, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_value_at(0, {6, 6}), 1023);
  EXPECT_EQ(image->get_value_at(1, {6, 6}), 1023);
  EXPECT_EQ(image->get_value_at(2, {6, 6}), 1023);
}


TEST(ImReadTests, ReadRGBImageCorrectValueInTheBlackBorder) {
  auto image = ImageIO::imread<RGBImage, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");
  EXPECT_EQ(image->get_value_at(0, {4, 6}), 0);
  EXPECT_EQ(image->get_value_at(1, {4, 6}), 0);
  EXPECT_EQ(image->get_value_at(2, {4, 6}), 0);
}


TEST(CLikeSyntaxTest, ReadBT601ImageCorrectType) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<BT601Image, uint16_t>(*image_file);
  EXPECT_EQ(image->get_type(), ImageType::BT601);
}


TEST(CLikeSyntaxTest, ReadBT709ImageCorrectType) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<BT709Image, uint16_t>(*image_file);
  EXPECT_EQ(image->get_type(), ImageType::BT709);
}


TEST(CLikeSyntaxTest, ReadBT2020ImageCorrectType) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<BT2020Image, uint16_t>(*image_file);
  EXPECT_EQ(image->get_type(), ImageType::BT2020);
}


TEST(CLikeSyntaxTest, ReadYCbCrImageCorrectWidth) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<BT601Image, uint16_t>(*image_file);
  EXPECT_EQ(image->get_width(), 13);
}


TEST(CLikeSyntaxTest, ReadYCbCrImageCorrectHeight) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<BT601Image, uint16_t>(*image_file);
  EXPECT_EQ(image->get_height(), 13);
}


TEST(CLikeSyntaxTest, ReadRGBImageCorrectType) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<RGBImage, uint16_t>(*image_file);
  EXPECT_EQ(image->get_type(), ImageType::RGB);
}


TEST(CLikeSyntaxTest, ReadRGBImageCorrectValueInTheMiddle) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<RGBImage, uint16_t>(*image_file);
  EXPECT_EQ(image->get_value_at(0, {6, 6}), 1023);
  EXPECT_EQ(image->get_value_at(1, {6, 6}), 1023);
  EXPECT_EQ(image->get_value_at(2, {6, 6}), 1023);
}


TEST(CLikeSyntaxTest, ReadRGBImageCorrectValueInTheBlackBorder) {
  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image = ImageIO::read<RGBImage, uint16_t>(*image_file);
  EXPECT_EQ(image->get_value_at(0, {4, 6}), 0);
  EXPECT_EQ(image->get_value_at(1, {4, 6}), 0);
  EXPECT_EQ(image->get_value_at(2, {4, 6}), 0);
}


TEST(CLikeSyntaxTest, ImReadAndOpenPlusReadMustResultInTheSameImage) {
  auto image_imread = ImageIO::imread<RGBImage, uint16_t>(
      resources_path + "/rgb_pattern/pattern.ppm");


  auto image_file = ImageIO::open(resources_path + "/rgb_pattern/pattern.ppm");
  auto image_read = ImageIO::read<RGBImage, uint16_t>(*image_file);

  EXPECT_EQ(*image_imread, *image_read);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}