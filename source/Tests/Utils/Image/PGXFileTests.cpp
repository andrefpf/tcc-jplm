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

/** \file     PGXFileTests.cpp
 *  \brief    
 *  \details  
 *  \author   iseidel <i.seidel@samsung.com>
 *  \date     2019-12-05
 */


#include <iostream>
#include "Lib/Utils/Image/PGXFile.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


struct PGXFileCheck : public testing::Test {
  std::string filename = "undefined.pgx";
  std::size_t width = 4;
  std::size_t height = 3;
  std::size_t depth = 12;
  bool is_signed = false;
  const std::streampos raster_begin =
      3;  //raster begin is not tested for a valid position
  PGXEndianess endianess = PGXEndianess::PGX_ML_BIG_ENDIAN;
};


TEST_F(PGXFileCheck, CanCreateAPGXFile) {
  [[maybe_unused]] auto pgx_file =
      PGXFile(filename, raster_begin, width, height, depth, is_signed, endianess);
}


TEST_F(PGXFileCheck, CanGetWidth) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed, endianess);
  EXPECT_EQ(width, pgx_file.get_width());
}


TEST_F(PGXFileCheck, CanGetHeight) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed, endianess);
  EXPECT_EQ(height, pgx_file.get_height());
}


TEST_F(PGXFileCheck, CanGetDepth) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed, endianess);
  EXPECT_EQ(depth, pgx_file.get_depth());
}


TEST_F(PGXFileCheck, CanGetSigned) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed, endianess);
  EXPECT_EQ(is_signed, pgx_file.is_signed());
}


TEST_F(PGXFileCheck, CanGetSignedFALSE) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, false, endianess);
  EXPECT_FALSE(pgx_file.is_signed());
}


TEST_F(PGXFileCheck, CanGetSignedTRUE) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, true, endianess);
  EXPECT_TRUE(pgx_file.is_signed());
}


TEST_F(PGXFileCheck, CanGetEndianess) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed, endianess);
  EXPECT_EQ(endianess, pgx_file.get_endianess());
}


TEST_F(PGXFileCheck, CanGetEndianessBig) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed,
      PGXEndianess::PGX_ML_BIG_ENDIAN);
  EXPECT_EQ(PGXEndianess::PGX_ML_BIG_ENDIAN, pgx_file.get_endianess());
}


TEST_F(PGXFileCheck, CanGetEndianessLittle) {
  auto pgx_file = PGXFile(filename, raster_begin, width, height, depth, is_signed,
      PGXEndianess::PGX_LM_LITTLE_ENDIAN);
  EXPECT_EQ(PGXEndianess::PGX_LM_LITTLE_ENDIAN, pgx_file.get_endianess());
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}