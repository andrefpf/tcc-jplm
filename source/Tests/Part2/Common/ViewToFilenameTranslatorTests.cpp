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

/** \file     ViewToFilenameTranslatorTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-12
 */

#include <iostream>
#include "Lib/Part2/Common/ViewToFilenameTranslator.h"
#include "gtest/gtest.h"


TEST(ViewToFilenameTranslatorBasicTests, ZeroZeroView) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "000_000.ppm", filename_translator.view_position_to_filename({0, 0}));
}


TEST(ViewToFilenameTranslatorBasicTests, ZeroOneView) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "000_001.ppm", filename_translator.view_position_to_filename({0, 1}));
}


TEST(ViewToFilenameTranslatorBasicTests, OneZeroView) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "001_000.ppm", filename_translator.view_position_to_filename({1, 0}));
}


TEST(ViewToFilenameTranslatorBasicTests, OneOneView) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "001_001.ppm", filename_translator.view_position_to_filename({1, 1}));
}


TEST(ViewToFilenameTranslatorBasicTests, One42View) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "001_042.ppm", filename_translator.view_position_to_filename({1, 42}));
}


TEST(ViewToFilenameTranslatorBasicTests, Fourty42View) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "042_042.ppm", filename_translator.view_position_to_filename({42, 42}));
}


TEST(ViewToFilenameTranslatorBasicTests, MaxView) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_EQ(
      "999_999.ppm", filename_translator.view_position_to_filename({999, 999}));
}


TEST(ViewToFilenameTranslatorBasicTests, NegativeTValueThrowsOverflow) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_THROW(filename_translator.view_position_to_filename({-1, 1}),
      ViewToFilenameTranslatorExceptions::Char3OverflowException);
}


TEST(ViewToFilenameTranslatorBasicTests, NegativeSValueThrowsOverflow) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_THROW(filename_translator.view_position_to_filename({1, -1}),
      ViewToFilenameTranslatorExceptions::Char3OverflowException);
}


TEST(ViewToFilenameTranslatorBasicTests, NegativeTandSValueThrowsOverflow) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_THROW(filename_translator.view_position_to_filename({-1, -1}),
      ViewToFilenameTranslatorExceptions::Char3OverflowException);
}


TEST(ViewToFilenameTranslatorBasicTests, LargerThan999TValueThrowsOverflow) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_THROW(filename_translator.view_position_to_filename({1000, 1}),
      ViewToFilenameTranslatorExceptions::Char3OverflowException);
}


TEST(ViewToFilenameTranslatorBasicTests, LargerThan999SValueThrowsOverflow) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_THROW(filename_translator.view_position_to_filename({1, 1000}),
      ViewToFilenameTranslatorExceptions::Char3OverflowException);
}


TEST(
    ViewToFilenameTranslatorBasicTests, LargerThan999TandSValueThrowsOverflow) {
  PPM3CharViewToFilename filename_translator;
  EXPECT_THROW(filename_translator.view_position_to_filename({1000, 1000}),
      ViewToFilenameTranslatorExceptions::Char3OverflowException);
}


TEST(ViewToFilenameTranslatorBasicTests, ValidPositionsDoNotThrow) {
  PPM3CharViewToFilename filename_translator;
  for (auto t = 0; t < 1000; ++t) {
    for (auto s = 0; s < 1000; ++s) {
      EXPECT_NO_THROW(filename_translator.view_position_to_filename({t, s}));
    }
  }
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
