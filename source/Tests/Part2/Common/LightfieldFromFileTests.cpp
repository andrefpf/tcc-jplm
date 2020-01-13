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

/** \file     LightfieldFromFileTests.cpp
 *  \brief    Test of LightfieldFromFile.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-06
 */


#include <iostream>
#include "Lib/Part2/Common/LightfieldFromFile.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


struct LFFromPPMSmallGreek : public testing::Test {
 protected:
  LightfieldDimension<std::size_t> size;
  LightfieldCoordinate<std::size_t> initial;
  LightfieldFromFile<uint16_t> lightfield;

 public:
  LFFromPPMSmallGreek()
      : size(3, 3, 32, 32), initial(0, 0, 0, 0),
        lightfield({{resources_path + "/small_greek/"}, initial, size}){};
  ~LFFromPPMSmallGreek() = default;
};


TEST_F(LFFromPPMSmallGreek, HasCorrectWidth) {
  auto width = lightfield.get_width();
  EXPECT_EQ(width, 3);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectHeight) {
  auto height = lightfield.get_height();
  EXPECT_EQ(height, 3);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectViewWidth) {
  auto width = lightfield.get_views_width_u();
  EXPECT_EQ(width, 32);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectViewHeight) {
  auto height = lightfield.get_views_height_v();
  EXPECT_EQ(height, 32);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectViewBpp) {
  //notice that there is no bpp informed
  auto bpp = lightfield.get_views_bpp();
  EXPECT_EQ(bpp, 10);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectDimensions) {
  auto dimensions = lightfield.get_dimensions();
  EXPECT_EQ(dimensions, size);
}


TEST_F(LFFromPPMSmallGreek, ZeroZeroImageIsEqual) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/small_greek/000_000.ppm"});
  auto variadic_image = file_original->read_full_image();
  auto image = std::move(std::get<1>(variadic_image));
  EXPECT_EQ(*image, lightfield.get_image_at({0, 0}));
}


TEST_F(LFFromPPMSmallGreek, ZeroZeroImageIsDifferentFromOneOneImage) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/small_greek/001_001.ppm"});
  auto variadic_image = file_original->read_full_image();
  auto image = std::move(std::get<1>(variadic_image));
  EXPECT_NE(*image, lightfield.get_image_at({0, 0}));
}


TEST_F(LFFromPPMSmallGreek, OneOneImageIsEqual) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/small_greek/001_001.ppm"});
  auto variadic_image = file_original->read_full_image();
  auto image = std::move(std::get<1>(variadic_image));
  EXPECT_EQ(*image, lightfield.get_image_at({1, 1}));
}


TEST_F(LFFromPPMSmallGreek, AllImagesAreEqualTheirExpectedImage) {
  PPM3CharViewToFilename filename_translator;
  for (auto t = 0; t < 3; ++t) {
    for (auto s = 0; s < 3; ++s) {
      auto file_original = PixelMapFileIO::open(
          {resources_path + "/small_greek/" +
              filename_translator.view_position_to_filename({t, s})});
      auto variadic_image = file_original->read_full_image();
      auto image = std::move(std::get<1>(variadic_image));
      EXPECT_EQ(*image, lightfield.get_image_at({t, s}));
    }
  }
}


TEST_F(LFFromPPMSmallGreek,
    TranposingTheIndicesGetsWrongImagesButTheMainDiagonal) {
  PPM3CharViewToFilename filename_translator;
  for (auto t = 0; t < 3; ++t) {
    for (auto s = 0; s < 3; ++s) {
      auto file_original = PixelMapFileIO::open(
          {resources_path + "/small_greek/" +
              filename_translator.view_position_to_filename({s, t})});
      auto variadic_image = file_original->read_full_image();
      auto image = std::move(std::get<1>(variadic_image));
      if (s == t) {
        EXPECT_EQ(*image, lightfield.get_image_at({t, s}));
      } else {
        EXPECT_NE(*image, lightfield.get_image_at({t, s}));
      }
    }
  }
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}
