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

/** \file     JPLFileTests.cpp
 *  \brief    Tests of JPLFile.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-07
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include "Lib/Part1/Common/JPLFile.h"
#include "gtest/gtest.h"


TEST(JPLFileBasicTest, JPLFileInitializationDoesNotThrow) {
  auto file_type_box = FileTypeBox(JpegPlenoFileTypeContents());
  EXPECT_NO_THROW(auto jpl_file = JPLFile(file_type_box));
}


TEST(JPLFileBasicTest, JPLFileHasValidSignatureBox) {
  auto file_type_box = FileTypeBox(JpegPlenoFileTypeContents());
  auto jpl_file = JPLFile(file_type_box);
  EXPECT_EQ(jpl_file.get_jpeg_pleno_signature_box(), JpegPlenoSignatureBox());
}


TEST(JPLFileBasicTest, JPLFileIsCompatibleWithPleno) {
  auto file_type_box = FileTypeBox(JpegPlenoFileTypeContents());
  auto jpl_file = JPLFile(file_type_box);
  EXPECT_TRUE(jpl_file.get_file_type_box().get_ref_to_contents().is_the_file_compatible_with(0x6a706c20));
}


TEST(JPLFileBasicTest, JPLFileCanHoldAThumbnailBox) {
  auto file_type_box = FileTypeBox(JpegPlenoFileTypeContents());
  auto jpl_file = JPLFile(file_type_box);
  // jpl_file.add_thumbnail_box(JpegPlenoThumbnailBox(JpegPlenoThumbnailContents()));
  // EXPECT_TRUE(jpl_file.has_thumbnail());
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}