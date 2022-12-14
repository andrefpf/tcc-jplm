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

/** \file     PGXFileIOTests.cpp
 *  \brief    
 *  \details  
 *  \author   iseidel <i.seidel@samsung.com>
 *  \date     2019-12-05
 */


#include <iostream>
#include "Lib/Utils/Image/ImageExceptions.h"
#include "Lib/Utils/Image/PGXFileIO.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


struct PGXFileCheck : public testing::Test {
  std::string filename =
      std::string(resources_path + "/pgx_tests/unsigned_big_endian_4x3x12.pgx");
  std::size_t width = 4;
  std::size_t height = 3;
  std::size_t depth = 12;
  bool is_signed = false;
  PGXEndianess endianess = PGXEndianess::PGX_ML_BIG_ENDIAN;
};


TEST_F(PGXFileCheck, CanUnderstandAValidId) {
  EXPECT_NO_THROW(auto pgx_file = PGXFileIO::open(filename));
}


TEST_F(PGXFileCheck, ThrowsForInvalidId) {
  EXPECT_THROW(auto pgx_file = PGXFileIO::open(
                   resources_path + "/pgx_tests/invalid_id.pgx"),
      PGXFileExceptions::InvalidIdException);
}


TEST_F(PGXFileCheck, GetsNotNullPGXfile) {
  auto pgx_file = PGXFileIO::open(filename);
  EXPECT_TRUE(pgx_file);
}


TEST_F(PGXFileCheck, GetsEndianessFromFileBigEndian) {
  auto pgx_file = PGXFileIO::open(filename);
  EXPECT_EQ(endianess, pgx_file->get_endianess());
}


TEST_F(PGXFileCheck, GetsEndianessFromFileLittleEndian) {
  auto pgx_file = PGXFileIO::open(
      resources_path + "/pgx_tests/unsigned_little_endian_4x3x12.pgx");
  EXPECT_EQ(PGXEndianess::PGX_LM_LITTLE_ENDIAN, pgx_file->get_endianess());
}


TEST_F(PGXFileCheck, ThrowsForInvalidEndianess) {
  EXPECT_THROW(
      auto pgx_file = PGXFileIO::open(
          resources_path + "/pgx_tests/unsigned_invalid_endian_4x3x12.pgx"),
      PGXFileExceptions::InvalidEndianessException);
}


TEST_F(PGXFileCheck, GetsIsSignedFieldFromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(filename);
  EXPECT_EQ(is_signed, pgx_file->is_signed());
}


TEST_F(PGXFileCheck, GetsIsSignedFieldFromFileSigned) {
  auto pgx_file = PGXFileIO::open(
      resources_path + "/pgx_tests/signed_big_endian_4x3x12.pgx");
  EXPECT_EQ(true, pgx_file->is_signed());
}


TEST_F(PGXFileCheck, ThrowsForInvalidSign) {
  EXPECT_THROW(
      auto pgx_file = PGXFileIO::open(
          resources_path + "/pgx_tests/invalid_sign_big_endian_4x3x12.pgx"),
      PGXFileExceptions::InvalidSignFieldException);
}


TEST_F(PGXFileCheck, GetsDepthFromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(filename);
  EXPECT_EQ(depth, pgx_file->get_depth());
}


TEST_F(PGXFileCheck, GetsDepth10FromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(resources_path + "/pgx_tests/unsigned_big_endian_3x4x10.pgx");
  EXPECT_EQ(10, pgx_file->get_depth());
}


TEST_F(PGXFileCheck, GetsWidthFromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(filename);
  EXPECT_EQ(width, pgx_file->get_width());
}


TEST_F(PGXFileCheck, GetsWidth3FromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(resources_path + "/pgx_tests/unsigned_big_endian_3x4x10.pgx");
  EXPECT_EQ(3, pgx_file->get_width());
}


TEST_F(PGXFileCheck, GetsHeightFromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(filename);
  EXPECT_EQ(height, pgx_file->get_height());
}


TEST_F(PGXFileCheck, GetsHeight4FromFileUnsigned) {
  auto pgx_file = PGXFileIO::open(resources_path + "/pgx_tests/unsigned_big_endian_3x4x10.pgx");
  EXPECT_EQ(4, pgx_file->get_height());
}



int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}