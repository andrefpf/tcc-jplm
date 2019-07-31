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

/** \file     JpegPlenoFileTypeContentsTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-26
 */


#include <iostream>
#include "Lib/Part1/Common/JpegPlenoFileTypeContents.h"
#include "Lib/Part1/Common/FileTypeBox.h"
#include "gtest/gtest.h"


TEST(JpegPlenoFileTypeContentsBasics, InitializationDoesNotThrow) {
  EXPECT_NO_THROW(
      auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents());
}


TEST(JpegPlenoFileTypeContentsBasics, GetsCorrectBrand) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  EXPECT_EQ(jpeg_pleno_file_type_contents.get_brand(), 0x6a706c20);
}


TEST(JpegPlenoFileTypeContentsBasics,
    DefaultConstructorGeneratesAsMinorVersion0) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  EXPECT_EQ(jpeg_pleno_file_type_contents.get_minor_version(), 0);
}


TEST(JpegPlenoFileTypeContentsBasics,
    DefaultConstructorGeneratesACompatibilityListWithOneItem) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  EXPECT_EQ(jpeg_pleno_file_type_contents.get_reference_to_compatibility_list()
                .size(),
      1);
}


TEST(JpegPlenoFileTypeContentsBasics,
    DefaultConstructorGeneratesACompatibilityListWithOneCompatibleStandard) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  EXPECT_EQ(
      jpeg_pleno_file_type_contents.get_number_of_compatible_standards(), 1);
}


TEST(JpegPlenoFileTypeContentsBasics, CompatibleWithItsOwnBrand) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  EXPECT_TRUE(jpeg_pleno_file_type_contents.is_the_file_compatible_with(
      jpeg_pleno_file_type_contents.get_brand()));
}


TEST(JpegPlenoFileTypeContentsBasics, BytesSize) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  auto bytes = jpeg_pleno_file_type_contents.get_bytes();

  EXPECT_EQ(bytes.size(), 12);
}


TEST(JpegPlenoFileTypeContentsBasics, BytesBrand) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  auto bytes = jpeg_pleno_file_type_contents.get_bytes();

  EXPECT_EQ(bytes.at(0), std::byte{0x6a});
  EXPECT_EQ(bytes.at(1), std::byte{0x70});
  EXPECT_EQ(bytes.at(2), std::byte{0x6c});
  EXPECT_EQ(bytes.at(3), std::byte{0x20});
}


TEST(JpegPlenoFileTypeContentsBasics, BytesMinV) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  auto bytes = jpeg_pleno_file_type_contents.get_bytes();

  //the value of this field shall be zero
  EXPECT_EQ(bytes.at(4), std::byte{0x00});
  EXPECT_EQ(bytes.at(5), std::byte{0x00});
  EXPECT_EQ(bytes.at(6), std::byte{0x00});
  EXPECT_EQ(bytes.at(7), std::byte{0x00});
}


TEST(JpegPlenoFileTypeContentsBasics, BytesCompatibility) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  auto bytes = jpeg_pleno_file_type_contents.get_bytes();

  //for the standard constructor it adds 0x6a706c20 as compatible 
  //right after MinV
  EXPECT_EQ(bytes.at(8), std::byte{0x6a});
  EXPECT_EQ(bytes.at(9), std::byte{0x70});
  EXPECT_EQ(bytes.at(10), std::byte{0x6c});
  EXPECT_EQ(bytes.at(11), std::byte{0x20});
}


TEST(JpegPlenoFileTypeContentsBasics, BytesCompatibleWithItsBrand) {
  auto jpeg_pleno_file_type_contents = JpegPlenoFileTypeContents();
  auto bytes = jpeg_pleno_file_type_contents.get_bytes();

  //for the standard constructor it adds 0x6a706c20 as compatible 
  //right after MinV
  EXPECT_EQ(bytes.at(0), bytes.at(8));
  EXPECT_EQ(bytes.at(1), bytes.at(9));
  EXPECT_EQ(bytes.at(2), bytes.at(10));
  EXPECT_EQ(bytes.at(3), bytes.at(11));
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}