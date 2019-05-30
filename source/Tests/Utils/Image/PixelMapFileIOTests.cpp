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

/** \file     PixelMapFileIOTests.cpp
 *  \brief    Test of PPM Binary File reader/writer.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-29
 */

#include <iostream>
#include <string>
#include "PixelMapFileIO.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(InitialTest, OppeningValidPathReturnsANonNullUniquePtr) {
  auto pixel_map_file =
      PixelMapFileIO::open({resources_path + "/rgb_pattern/pattern.ppm"});
  EXPECT_NE(pixel_map_file.get(), nullptr);
}

/***
EXCEPTIONS TESTING
**/

TEST(ExceptionsTestWhenOpeningForRead, InexistentPathMustThrowException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/inexistent_path/pattern.ppm"}),
      ImageIOExceptions::InexistentPathException);
}


TEST(ExceptionsTestWhenOpeningForRead, InexistentFileMustThrowException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/inexistent_file.ppm"}),
      ImageIOExceptions::InexistentFileException);
}


TEST(ExceptionsTestWhenOpeningForRead, UnknownImageTypeMustThrowException) {
  EXPECT_THROW(
      auto pixel_map_file = PixelMapFileIO::open(
          {resources_path + "/pixmap_exceptions/unknown_file.unknown"}),
      ImageIOExceptions::UnknownImageTypeException);
}


TEST(ExceptionsTestWhenOpeningForRead, NotARegularFileMustThrowException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/not_regular_file"}),
      ImageIOExceptions::NotARegularFileException);
}


TEST(ExceptionsTestWhenOpeningForRead,
    InvalidPixMapIndexLargerThan6MustThrowException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/invalid_p7.ppm"}),
      PixelMapFileIOExceptions::InvalidPixelMapIndexException);
}


TEST(ExceptionsTestWhenOpeningForRead,
    InvalidPixMapIndexSmallerThan1MustThrowException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/invalid_p0.ppm"}),
      PixelMapFileIOExceptions::InvalidPixelMapIndexException);
}


TEST(ExceptionsTestWhenOpeningForRead, InvalidPixMapBpp0MustThrowException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/invalid_bpp0.ppm"}),
      PixelMapFileIOExceptions::BppMustBeLargerThanZeroException);
}


TEST(ExceptionsTestWhenOpeningForRead,
    InvalidPixMapBppLargerThan16ThrowsException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/invalid_bpp17.ppm"}),
      PixelMapFileIOExceptions::BppMustBeSmallerOrEqualToSixteenException);
}


TEST(ExceptionsTestWhenOpeningForRead, InvalidPixMapZeroWidthThrowsException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/invalid_width0.ppm"}),
      PixelMapFileIOExceptions::WidthMustBeLargerThanZeroException);
}


TEST(ExceptionsTestWhenOpeningForRead, InvalidPixMapZeroHeightThrowsException) {
  EXPECT_THROW(auto pixel_map_file = PixelMapFileIO::open(
                   {resources_path + "/pixmap_exceptions/invalid_height0.ppm"}),
      PixelMapFileIOExceptions::HeightMustBeLargerThanZeroException);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}