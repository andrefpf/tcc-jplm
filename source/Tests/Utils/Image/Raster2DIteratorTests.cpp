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

/** \file     Raster2DIteratorTests.cpp
 *  \brief    Test of lightfield.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-12
 *
 */

#include <iostream>
#include "Lib/Utils/Image/Raster2DIterator.h"
#include "gtest/gtest.h"


struct Raster2DIteratorTest : public testing::Test {
  int test[12];
  Raster2DIterator<int> test_begin;
  Raster2DIterator<int> test_end;
  Raster2DIteratorTest() : test_begin(&test[0]), test_end(&test[11] + 1) {
    for (auto i = 0; i < 12; ++i) {
      test[i] = 0;
    }
  };
};


TEST_F(Raster2DIteratorTest, IteratorBeginHasInitialValue) {
  test[0] = 42;
  EXPECT_EQ(*test_begin, 42);
}


TEST_F(Raster2DIteratorTest, IteratorBeginPlusOneHasInitialValueOfIndexOne) {
  test[1] = 40;
  test_begin++;
  EXPECT_EQ(*(test_begin), 40);
}


TEST_F(
    Raster2DIteratorTest, IteratorBeginPostIncrementHasInitialValueOfIndexOne) {
  test[0] = 42;
  test[1] = 40;
  EXPECT_EQ(*(test_begin++), 42);
}


TEST_F(
    Raster2DIteratorTest, IteratorBeginPreIncrementHasInitialValueOfIndexOne) {
  test[0] = 42;
  test[1] = 40;
  EXPECT_EQ(*(++test_begin), 40);
}


TEST_F(Raster2DIteratorTest, IteratorEndMinusOneHasTheLastValue) {
  test[11] = 40;
  EXPECT_EQ(*(--test_end), 40);
}


TEST_F(Raster2DIteratorTest,
    IteratorEndPostDecrementValueIsEqualAsItWasBeforeDecrement) {
  auto end = test_end;  //copy assignment
  EXPECT_EQ(test_end--, end);
}


TEST_F(Raster2DIteratorTest, ReadingOfVectorIsCorrectInBeginEndLoop) {
  for (auto i = 0; i < 12; ++i) {
    test[i] = i;
  }
  auto temp = 0;
  for (auto it = test_begin; it != test_end; ++it) {
    EXPECT_EQ(*it, temp++);
  }
}


TEST_F(Raster2DIteratorTest, WritingOfVectorIsCorrectInBeginEndLoop) {
  auto temp = 0;
  for (auto it = test_begin; it != test_end; ++it) {
    *it = temp++;
  }
  for (auto i = 0; i < 12; ++i) {
    EXPECT_EQ(i, test[i]);
  }
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
