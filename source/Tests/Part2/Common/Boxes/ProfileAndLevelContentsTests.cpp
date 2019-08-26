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

/** \file     ProfileAndLevelContentsTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */


#include <iostream>
#include "Lib/Part2/Common/Boxes/ProfileAndLevelContents.h"
#include "gtest/gtest.h"


TEST(BasicTests, NoThrow) {
  EXPECT_NO_THROW(
      auto profile_and_level_contents = ProfileAndLevelContents(42, 24));
}


TEST(BasicTests, GetsProfile) {
  auto profile_and_level_contents = ProfileAndLevelContents(42, 24);
  EXPECT_EQ(profile_and_level_contents.get_profile_of_the_codestream(), 42);
}


TEST(BasicTests, GetsPpih) {
  auto profile_and_level_contents = ProfileAndLevelContents(42, 24);
  EXPECT_EQ(profile_and_level_contents.get_ppih(), 42);
}


TEST(BasicTests, ProfileIsPpih) {
  auto profile_and_level_contents = ProfileAndLevelContents(42, 24);
  EXPECT_EQ(profile_and_level_contents.get_profile_of_the_codestream(),
      profile_and_level_contents.get_ppih());
}


TEST(BasicTests, GetsLevel) {
  auto profile_and_level_contents = ProfileAndLevelContents(42, 24);
  EXPECT_EQ(profile_and_level_contents.get_level_of_the_codestream(), 24);
}


TEST(BasicTests, GetsPlev) {
  auto profile_and_level_contents = ProfileAndLevelContents(42, 24);
  EXPECT_EQ(profile_and_level_contents.get_plev(), 24);
}


TEST(BasicTests, LevelIsPlev) {
  auto profile_and_level_contents = ProfileAndLevelContents(42, 24);
  EXPECT_EQ(profile_and_level_contents.get_level_of_the_codestream(),
      profile_and_level_contents.get_plev());
}


TEST(BasicTests, Has4Bytes) {
  auto profile_and_level_contents = ProfileAndLevelContents(0, 0);
  EXPECT_EQ(profile_and_level_contents.size(), 4);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}