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

/** \file     PPMBinaryFileTests.cpp
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-29
 */

#include <iostream>
#include "ImageIO.h"
#include "PPMBinaryFile.h"
#include "PixelMapFileIO.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";


TEST(InitialTest, IsTypeP6) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/rgb_pattern/pattern.ppm"});
  EXPECT_EQ(PixelMapType::P6, file_original->get_type());
}


TEST(InitialTest, HasExpectedWidth) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/rgb_pattern/pattern.ppm"});
  EXPECT_EQ(13, file_original->get_width());
}


TEST(InitialTest, HasExpectedHeight) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/rgb_pattern/pattern.ppm"});
  EXPECT_EQ(13, file_original->get_height());
}


TEST(InitialTest, HasExpectedBpp) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/rgb_pattern/pattern.ppm"});
  EXPECT_EQ(10, file_original->get_number_of_bits_per_pixel());
}

uint16_t max_r_10_bit = 1023;
uint16_t max_g_10_bit = 1023;
uint16_t max_b_10_bit = 1023;

uint16_t half_r_10_bit = 511;
uint16_t half_g_10_bit = 511;
uint16_t half_b_10_bit = 511;

uint16_t min_r_10_bit = 0;
uint16_t min_g_10_bit = 0;
uint16_t min_b_10_bit = 0;

auto yellow_10_bit = std::make_tuple(max_r_10_bit, max_g_10_bit, min_b_10_bit);
auto red_10_bit = std::make_tuple(max_r_10_bit, min_g_10_bit, min_b_10_bit);
auto green_10_bit = std::make_tuple(min_r_10_bit, max_g_10_bit, min_b_10_bit);
auto blue_10_bit = std::make_tuple(min_r_10_bit, min_g_10_bit, max_b_10_bit);
auto white_10_bit = std::make_tuple(max_r_10_bit, max_g_10_bit, max_b_10_bit);
auto black_10_bit = std::make_tuple(min_r_10_bit, min_g_10_bit, min_b_10_bit);
auto gray_10_bit = std::make_tuple(half_r_10_bit, half_g_10_bit, half_b_10_bit);


struct PPMBinaryWriteTests : public testing::Test {
 protected:
  std::unique_ptr<RGBImage<uint16_t>> image =
      std::make_unique<RGBImage<uint16_t>>(13, 13, 10);
  const std::string output_filename =
      std::string({resources_path + "/test.ppm"});
  PPMBinaryWriteTests() {
    for (auto i = 0; i < 13; ++i) {
      for (auto j = 0; j < 13; ++j) {
        if (i == 0 || i == 12 || j == 0 || j == 12) {
          image->set_pixel_at(white_10_bit, i, j);
          continue;
        }
        if (i == 1 || i == 11 || j == 1 || j == 11) {
          image->set_pixel_at(blue_10_bit, i, j);
          continue;
        }
        if (i == 2 || i == 10 || j == 2 || j == 10) {
          image->set_pixel_at(green_10_bit, i, j);
          continue;
        }
        if (i == 3 || i == 9 || j == 3 || j == 9) {
          image->set_pixel_at(red_10_bit, i, j);
          continue;
        }
        if (i == 4 || i == 8 || j == 4 || j == 8) {
          image->set_pixel_at(black_10_bit, i, j);
          continue;
        }
        if (i == 5 || i == 7 || j == 5 || j == 7) {
          image->set_pixel_at(gray_10_bit, i, j);
          continue;
        }
        image->set_pixel_at(yellow_10_bit, i, j);
      }
    }
  }

  ~PPMBinaryWriteTests() {
  	namespace fs = std::filesystem;
    if (fs::exists(output_filename)) {
      fs::remove(output_filename);
    }
  }
};


TEST_F(PPMBinaryWriteTests, ImageWriteCreatesAFile) {
  namespace fs = std::filesystem;
  if (fs::exists(output_filename)) {
    fs::remove(output_filename);
  }
  EXPECT_FALSE(fs::exists(output_filename));
  ImageIO::write(*(image.get()), output_filename);
  EXPECT_TRUE(fs::exists(output_filename));
}


TEST_F(PPMBinaryWriteTests, ImageWriteOverExistingFileThrows) {
  namespace fs = std::filesystem;
  if (!fs::exists(output_filename)) {
    ImageIO::write(*(image.get()), output_filename);
    EXPECT_TRUE(fs::exists(output_filename));
  }

  EXPECT_THROW(ImageIO::write(*(image.get()), output_filename),
      ImageIOExceptions::FileAlreadyExistsException);
}


TEST_F(PPMBinaryWriteTests, ImageWriteCreatesAFileThatCanBeOppened) {
  namespace fs = std::filesystem;
  if (fs::exists(output_filename)) {
    fs::remove(output_filename);
  }
  EXPECT_FALSE(fs::exists(output_filename));
  ImageIO::write(*(image.get()), output_filename);
  auto ppm_file = PixelMapFileIO::open(output_filename);
  EXPECT_EQ(ppm_file->get_type(), PixelMapType::P6);
  fs::remove(output_filename);
}


TEST_F(PPMBinaryWriteTests, ImageWriteCreatesAFileThatContatinsSameDataAsTheWrittenImage) {
  namespace fs = std::filesystem;
  if (fs::exists(output_filename)) {
    fs::remove(output_filename);
  }
  EXPECT_FALSE(fs::exists(output_filename));
  ImageIO::write(*(image.get()), output_filename);
  auto ppm_file = PixelMapFileIO::open(output_filename);

  auto readed_image = ppm_file->read_full_image();
  ASSERT_EQ(readed_image.index(), 1);
  auto readed_16bpp_image = std::move(std::get<1>(readed_image));
  EXPECT_EQ(*(readed_16bpp_image.get()), *(image.get()));
  fs::remove(output_filename);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}