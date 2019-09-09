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

/** \file     ColorModelUtilsTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-13
 */


#include <iostream>
#include <random>
#include <tuple>
#include "Lib/Utils/Image/ColorModelUtils.h"
#include "gtest/gtest.h"

/** @addtogroup ImageLibTests
 *  Color Model Utils (Power of 2..., max ranges)
 *  @{
 */

TEST(PowerOf2IntegralType, TwoAtZeroIsOne) {
  auto two_at_zero = ColorModelUtils::power_of_2<std::size_t, 0>();
  EXPECT_EQ(1, two_at_zero);
}


TEST(PowerOf2IntegralType, TwoAtOneIsTwo) {
  auto two_at_one = ColorModelUtils::power_of_2<std::size_t, 1>();
  EXPECT_EQ(2, two_at_one);
}


TEST(PowerOf2IntegralType, TwoAtTwoIsFour) {
  auto two_at_two = ColorModelUtils::power_of_2<std::size_t, 2>();
  EXPECT_EQ(4, two_at_two);
}


TEST(PowerOf2IntegralType, TwoAtThreeIsFour) {
  auto two_at_three = ColorModelUtils::power_of_2<std::size_t, 3>();
  EXPECT_EQ(8, two_at_three);
}


TEST(PowerOf2IntegralType, TwoAtTenIsOneThousandAndTwentyFour) {
  auto two_at_ten = ColorModelUtils::power_of_2<std::size_t, 10>();
  EXPECT_EQ(1024, two_at_ten);
}


TEST(PowerOf2IntegralType, TwoAtKPlusOneIsTwoTimesTwoAtK) {
  auto two_at_nine = ColorModelUtils::power_of_2<std::size_t, 9>();
  auto two_at_ten = ColorModelUtils::power_of_2<std::size_t, 10>();
  EXPECT_EQ(2 * two_at_nine, two_at_ten);
}


TEST(PowerOf2FloatingPointType, TwoAtZeroIsOne) {
  auto two_at_zero = ColorModelUtils::power_of_2<double, 0>();
  EXPECT_EQ(1.0, two_at_zero);
}


TEST(PowerOf2FloatingPointType, TwoAtOneIsTwo) {
  auto two_at_one = ColorModelUtils::power_of_2<double, 1>();
  EXPECT_EQ(2.0, two_at_one);
}


TEST(PowerOf2FloatingPointType, TwoAtTwoIsFour) {
  auto two_at_two = ColorModelUtils::power_of_2<double, 2>();
  EXPECT_EQ(4.0, two_at_two);
}


TEST(PowerOf2FloatingPointType, TwoAtThreeIsFour) {
  auto two_at_three = ColorModelUtils::power_of_2<double, 3>();
  EXPECT_EQ(8.0, two_at_three);
}


TEST(PowerOf2FloatingPointType, TwoAtTenIsOneThousandAndTwentyFour) {
  auto two_at_ten = ColorModelUtils::power_of_2<double, 10>();
  EXPECT_EQ(1024.0, two_at_ten);
}


TEST(PowerOf2FloatingPointType, TwoAtKPlusOneIsTwoTimesTwoAtK) {
  auto two_at_nine = ColorModelUtils::power_of_2<double, 9>();
  auto two_at_ten = ColorModelUtils::power_of_2<double, 10>();
  EXPECT_EQ(2.0 * two_at_nine, two_at_ten);
}


TEST(Normalize, MinimumValueGoesToZeroUI8T) {
  uint8_t value = 0;
  auto normalized = ColorModelUtils::normalize01(value);
  EXPECT_EQ(0.0, normalized);
}


TEST(Normalize, MinimumValueGoesToZeroUI16T) {
  uint16_t value = 0;
  auto normalized = ColorModelUtils::normalize01<uint16_t, 8>(value);
  EXPECT_EQ(0.0, normalized);
}


TEST(Normalize, MaximumValueGoesToOneUI16T) {
  uint16_t value = 255;
  auto normalized = ColorModelUtils::normalize01<uint16_t, 8>(value);
  EXPECT_EQ(1.0, normalized);
}


TEST(Normalize, AboveMaximumValueGoesToOneUI16T) {
  uint16_t value = 256;
  auto normalized = ColorModelUtils::normalize01<uint16_t, 8>(value);
  EXPECT_EQ(1.0, normalized);
}


TEST(InverseNormalize, ZeroGoesToZeroUI8T) {
  double value = 0.0;
  auto inverse_normalized = ColorModelUtils::inverse_normalize01(value);
  EXPECT_EQ(0, inverse_normalized);
}


TEST(InverseNormalize, ZeroGoesToZeroUI16T) {
  double value = 0.0;
  auto inverse_normalized =
      ColorModelUtils::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(0, inverse_normalized);
}


TEST(InverseNormalize, BelowZeroGoesToZeroUI16T) {
  double value = -0.1;
  auto inverse_normalized =
      ColorModelUtils::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(0, inverse_normalized);
}


TEST(InverseNormalize, OneGoesToMaximumUI16T) {
  double value = 1.0;
  auto inverse_normalized =
      ColorModelUtils::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(255, inverse_normalized);
}


TEST(InverseNormalize, AboveOneGoesToMaximumValueUI16T) {
  double value = 1.25;
  auto inverse_normalized =
      ColorModelUtils::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(255, inverse_normalized);
}

 /**@}*/