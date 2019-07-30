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

/** \file     BinaryToolsTests.cpp
 *  \brief    Test of Binary Tools.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-30
 */

#include <iostream>
#include "Lib/Part1/Common/BinaryTools.h"
#include "gtest/gtest.h"


class UnchangedUint8_T_Test : public ::testing::TestWithParam<int> {
 public:
  UnchangedUint8_T_Test() = default;
  ~UnchangedUint8_T_Test() = default;
};


INSTANTIATE_TEST_SUITE_P(
    AUTO_GEN_OH, UnchangedUint8_T_Test, ::testing::Range(0, 255, 1));


TEST_P(UnchangedUint8_T_Test, uint8tKeepsEqualValue) {
  uint8_t value = static_cast<uint8_t>(GetParam());
  EXPECT_EQ(value, BinaryTools::ensure_machines_endianess(value));
}


class UInt16_T_Test : public ::testing::TestWithParam<int> {
 public:
  UInt16_T_Test() = default;
  ~UInt16_T_Test() = default;
};


INSTANTIATE_TEST_SUITE_P(AUTO_GEN_OH, UInt16_T_Test,
    ::testing::Range(
        0, static_cast<int>(std::numeric_limits<uint16_t>::max()), 11));


TEST_P(UInt16_T_Test, uint16tChangesBytesIfNotOnSameEndianess) {
  uint16_t value = static_cast<uint16_t>(GetParam());
  uint16_t inverted = 0;
  if (BinaryTools::using_little_endian()) {
    inverted = BinaryTools::ensure_big_endian<uint16_t, false>(value);
  } else {
    inverted = BinaryTools::ensure_big_endian<uint16_t, true>(value);
  }
  uint16_t expected = ((value & 0xFF00) >> 8) | ((value & 0x00FF) << 8);
  EXPECT_EQ(inverted, expected);
}



TEST(ValueFromByteVector, OneByteOnly) {
  auto bytes = std::vector<uint8_t>({255});
  auto value = BinaryTools::get_value_from_big_endian_byte_vector<uint8_t>(bytes);
  EXPECT_EQ(value, 255);
}


TEST(ValueFromByteVector, TwoBytes) {
  auto bytes = std::vector<uint8_t>({0, 255});
  auto value = BinaryTools::get_value_from_big_endian_byte_vector<uint16_t>(bytes);
  EXPECT_EQ(value, 255);
}


TEST(ValueFromByteVector, FourBytes) {
  auto bytes = std::vector<uint8_t>({0, 0, 0, 255});
  auto value = BinaryTools::get_value_from_big_endian_byte_vector<uint32_t>(bytes);
  EXPECT_EQ(value, 255);
}


TEST(ValueFromByteVector, FourBytesOtherValue) {
  auto bytes = std::vector<uint8_t>({0, 0, 255, 255});
  auto value = BinaryTools::get_value_from_big_endian_byte_vector<uint32_t>(bytes);
  EXPECT_EQ(value, 65535);
}

///! \todo need to test for exceptions


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}