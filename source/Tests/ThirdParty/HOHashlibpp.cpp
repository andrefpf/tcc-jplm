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

/** \file     HOHashlibpp.cpp
 *  \brief    Tests for the external library Header-Only Hashlib++
 *  \details  See more details about it at
 *            https://gitlab.com/pedrogarcia/ho-hashlibpp.git
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-04-06
 */

#include "gtest/gtest.h"
#include "ho-hashlibpp.h"

class HO_HashlibppStringTests : public ::testing::Test {
 protected:
  std::string input;

  void SetUp() override {
    input = "Hello World";
  }
};


TEST_F(HO_HashlibppStringTests, MD5FromStringTest) {
  std::string hash = HO_Hashlibpp::MD5(input);
  EXPECT_EQ("b10a8db164e0754105b7a99be72e3fe5", hash);
}


TEST_F(HO_HashlibppStringTests, SHA1FromStringTest) {
  std::string hash = HO_Hashlibpp::SHA1(input);
  EXPECT_EQ("0a4d55a8d778e5022fab701977c5d840bbc486d0", hash);
}


TEST_F(HO_HashlibppStringTests, SHA256FromStringTest) {
  std::string hash = HO_Hashlibpp::SHA256(input);
  EXPECT_EQ(
      "a591a6d40bf420404a011733cfb7b190d62c65bf0bcda32b57b277d9ad9f146e", hash);
}


TEST_F(HO_HashlibppStringTests, SHA384FromStringTest) {
  std::string hash = HO_Hashlibpp::SHA384(input);
  EXPECT_EQ(
      "99514329186b2f6ae4a1329e7ee6c610a729636335174ac6b740f9028396fcc803d0e93"
      "863a7c3d90f86beee782f4f3f",
      hash);
}


TEST_F(HO_HashlibppStringTests, SHA512FromStringTest) {
  std::string hash = HO_Hashlibpp::SHA512(input);
  EXPECT_EQ(
      "2c74fd17edafd80e8447b0d46741ee243b7eb74dd2149a0ab1b9246fb30382f27e853d8"
      "585719e0e67cbda0daa8f51671064615d645ae27acb15bfb1447f459b",
      hash);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
