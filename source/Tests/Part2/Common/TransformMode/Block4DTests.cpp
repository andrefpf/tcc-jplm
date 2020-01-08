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

/** \file     Block4DTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#include <iostream>
#include "Lib/Part2/Common/TransformMode/Block4D.h"
#include "gtest/gtest.h"


TEST(BasicTest, ConstructionDoesNotThrow) {
  EXPECT_NO_THROW(auto block4d = Block4D({1, 2, 3, 4}));
}


struct UnaryOperatorsTests : testing::Test {
 protected:
  Block4D block4d;
  UnaryOperatorsTests() {
    block4d.set_dimension(2, 2, 2, 2);
    for (auto i = 0; i < 16; ++i) {
      block4d.mPixelData[i] = i;
    }
  }
};


TEST_F(UnaryOperatorsTests, Block4DHasTheExpectedDimension) {
  const auto dimension = LightfieldDimension<uint32_t>(block4d.get_dimension());
  EXPECT_EQ(dimension, LightfieldDimension<uint32_t>(2, 2, 2, 2));
}


TEST_F(UnaryOperatorsTests, MultiplicationByScalarDouble) {
  block4d *= 2.0;
  for (auto i = 0; i < 16; ++i) {
    EXPECT_EQ(block4d.mPixelData[i], std::round(i * 2.0));
  }
}


TEST_F(UnaryOperatorsTests, DivisionByScalarDouble) {
  block4d /= 2.0;
  for (auto i = 0; i < 16; ++i) {
    EXPECT_EQ(block4d.mPixelData[i], std::round(i / 2.0));
  }
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}