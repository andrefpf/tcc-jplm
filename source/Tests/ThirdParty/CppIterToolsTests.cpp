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

/** \file     CppIterTools.cpp
 *  \brief    Tests for the external library CppIterTools
 *  \details  See more details about it at
 *            https://github.com/ryanhaining/cppitertools
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-06-18
 */


#include <iostream>
#include "cppitertools/range.hpp"
#include "cppitertools/accumulate.hpp"
#include "cppitertools/chain.hpp"
#include "gtest/gtest.h"

using iter::range;

TEST(CppIterTools, TestRangeOneArgument) {
  auto r = range(3);
  EXPECT_EQ(r.start(), 0);
  EXPECT_EQ(r.stop(), 3);
  EXPECT_EQ(r.step(), 1);

  // make sure iterators aren't changing the value
  auto it = r.begin();
  ++it;

  EXPECT_EQ(r.start(), 0);
  EXPECT_EQ(r.stop(), 3);
  EXPECT_EQ(r.step(), 1);
}

TEST(CppIterTools, TestRangeTwoArguments) {
  auto r = range(2, 10);
  EXPECT_EQ(r.start(), 2);
  EXPECT_EQ(r.stop(), 10);
  EXPECT_EQ(r.step(), 1);
}

TEST(CppIterTools, TestRangeThreeArguments) {
  auto r = range(-6, 20, 3);
  EXPECT_EQ(r.start(), -6);
  EXPECT_EQ(r.stop(), 20);
  EXPECT_EQ(r.step(), 3);
}

TEST(CppIterTools, TestRangeOneArgumentDouble) {
  auto r = range(3.5);
  EXPECT_DOUBLE_EQ(r.start(), 0);
  EXPECT_DOUBLE_EQ(r.stop(), 3.5);
  EXPECT_DOUBLE_EQ(r.step(), 1.0);
}

TEST(CppIterTools, TestRangeTwoArgumentDouble) {
  auto r = range(20.1, 31.7);
  EXPECT_DOUBLE_EQ(r.start(), 20.1);
  EXPECT_DOUBLE_EQ(r.stop(), 31.7);
  EXPECT_DOUBLE_EQ(r.step(), 1.0);
}

TEST(CppIterTools, TestRangeThreeArgumentDouble) {
  auto r = range(-6.3, 5.7, 0.1);
  EXPECT_DOUBLE_EQ(r.start(), -6.3);
  EXPECT_DOUBLE_EQ(r.stop(), 5.7);
  EXPECT_DOUBLE_EQ(r.step(), 0.1);
}

TEST(CppIterTools, TestRangeWithOnlyStop) {
  auto r = range(5);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{0, 1, 2, 3, 4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWithStartAndStop) {
  auto r = range(1, 5);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{1, 2, 3, 4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWithPositiveSteps) {
  auto r = range(1, 10, 3);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{1, 4, 7};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWithEmptyRange) {
  auto r = iter::range(0);
  EXPECT_TRUE(std::begin(r) == std::end(r));
}

TEST(CppIterTools, TestRangeWithPostfixIncrement) {
  auto r = iter::range(3);
  auto it = std::begin(r);
  it++;
  EXPECT_TRUE(*it == 1);
}

TEST(CppIterTools, TestRangeStartBiggerThanEnd) {
  auto r = range(5, 0);
  const std::vector<int> v(std::begin(r), std::end(r));
  EXPECT_TRUE(v.empty());
}

TEST(CppIterTools, TestRangeWithNegativeSteps) {
  auto r = range(5, -5, -3);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{5, 2, -1, -4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWhenStepNotDivideRangeEqually) {
  auto r = range(0, 5, 2);
  const std::vector<int> v(std::begin(r), std::end(r));
  const std::vector<int> vc{0, 2, 4};
  EXPECT_TRUE(v == vc);
}

TEST(CppIterTools, TestRangeWhenStepIsBiggerThanRange) {
  auto r = range(0, 10, 20);
  const std::vector<int> v(std::begin(r), std::end(r));
  EXPECT_TRUE(v.size() == 1);
}

TEST(CppIterTools, TestRangeWhenStepIsZero) {
  auto r = range(0, 10, 0);
  EXPECT_TRUE(std::begin(r) == std::end(r));
  auto r2 = range(0, -10, 0);
  EXPECT_TRUE(std::begin(r2) == std::end(r2));
}

TEST(CppIterTools, TestConstExprRange) {
  constexpr auto r = range(10);
  (void) r;
  constexpr auto r2 = range(4, 10);
  (void) r2;
  constexpr auto r3 = range(4, 10, 2);
  (void) r3;

  constexpr auto it = r2.begin();  // std::begin isn't constexpr
  constexpr auto i = *it;
  EXPECT_TRUE(i == 4);

  constexpr auto rf = range(10.0);
  constexpr auto itf = rf.begin();
  constexpr auto f = *itf;
  EXPECT_TRUE(f == 0.0);
}

TEST(CppIterTools, TestIndexedRange) {
  auto r = range(4);
  EXPECT_EQ(r[0], 0);
  EXPECT_EQ(r[1], 1);
  EXPECT_EQ(r[2], 2);
  EXPECT_EQ(r[3], 3);
}

TEST(CppIterTools, TestIndexedRangeWithTwoArguments) {
  auto r = range(10, 14);
  EXPECT_EQ(r[0], 10);
  EXPECT_EQ(r[1], 11);
  EXPECT_EQ(r[2], 12);
  EXPECT_EQ(r[3], 13);
}

TEST(CppIterTools, TestIndexedRangeWithThreeArguments) {
  auto r = range(10, 20, 3);
  EXPECT_EQ(r[0], 10);
  EXPECT_EQ(r[1], 13);
  EXPECT_EQ(r[2], 16);
  EXPECT_EQ(r[3], 19);
}

TEST(CppIterTools, TestIndexedRangeWithThreeDoubleArguments) {
  auto r = range(0.0, 0.99, 0.2);
  EXPECT_DOUBLE_EQ(r[0], 0.0);
  EXPECT_DOUBLE_EQ(r[1], 0.2);
  EXPECT_DOUBLE_EQ(r[2], 0.4);
  EXPECT_DOUBLE_EQ(r[3], 0.6);
  EXPECT_DOUBLE_EQ(r[4], 0.8);
}



int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
