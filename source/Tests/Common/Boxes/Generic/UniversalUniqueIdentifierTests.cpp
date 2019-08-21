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

/** \file     UniversalUniqueIdentifierTests.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-26
 */

#include <iostream>
#include "Lib/Common/Boxes/Generic/UniversalUniqueIdentifier.h"
#include "gtest/gtest.h"


TEST(UniversalUniqueIdentifierBasic, Initialization) {
  EXPECT_NO_THROW(UniversalUniqueIdentifier());
}


struct UniversalUniqueIdentifierFields : public testing::Test {
 protected:
  std::unique_ptr<UniversalUniqueIdentifier> uuid;
  my_uint48_t node = {1, 5819};
  uint8_t clock_seq_low = 10;
  uint8_t clock_seq_hi_and_reserved = 12;
  uint16_t time_hi_and_version = 14;
  uint16_t time_mid = 16;
  uint32_t time_low = 18;


 public:
  UniversalUniqueIdentifierFields() {
    uuid = std::make_unique<UniversalUniqueIdentifier>(node, clock_seq_low,
        clock_seq_hi_and_reserved, time_hi_and_version, time_mid, time_low);
  }

  ~UniversalUniqueIdentifierFields() = default;
};


TEST_F(UniversalUniqueIdentifierFields, HexStringHasTheCorrectNumberOfChars) {
  EXPECT_EQ(uuid->to_hex_string().size(), 36);
}


TEST_F(UniversalUniqueIdentifierFields, HexStringHasHyphensInTheRightPlaces) {
  EXPECT_EQ(uuid->to_hex_string().at(8), '-');
  EXPECT_EQ(uuid->to_hex_string().at(13), '-');
  EXPECT_EQ(uuid->to_hex_string().at(18), '-');
  EXPECT_EQ(uuid->to_hex_string().at(23), '-');
}


// TEST_F
//e7dca0b0-afb8-11e9-bd11-31bc71fe99d2


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}