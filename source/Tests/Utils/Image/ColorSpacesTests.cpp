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

/** \file     ColorSpacesTests.cpp
 *  \brief    Color Space conversion class
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-02-19
 */


#include <iostream>
#include "ColorSpaces.h"
#include "gtest/gtest.h"


TEST(PowerOf2IntegralType, TwoAtZeroIsOne) {
  auto two_at_zero = ColorSpaces::power_of_2<std::size_t, 0>();
  EXPECT_EQ(1, two_at_zero);
}


TEST(PowerOf2IntegralType, TwoAtOneIsTwo) {
  auto two_at_one = ColorSpaces::power_of_2<std::size_t, 1>();
  EXPECT_EQ(2, two_at_one);
}


TEST(PowerOf2IntegralType, TwoAtTwoIsFour) {
  auto two_at_two = ColorSpaces::power_of_2<std::size_t, 2>();
  EXPECT_EQ(4, two_at_two);
}


TEST(PowerOf2IntegralType, TwoAtThreeIsFour) {
  auto two_at_three = ColorSpaces::power_of_2<std::size_t, 3>();
  EXPECT_EQ(8, two_at_three);
}


TEST(PowerOf2IntegralType, TwoAtTenIsOneThousandAndTwentyFour) {
  auto two_at_ten = ColorSpaces::power_of_2<std::size_t, 10>();
  EXPECT_EQ(1024, two_at_ten);
}


TEST(PowerOf2IntegralType, TwoAtKPlusOneIsTwoTimesTwoAtK) {
  auto two_at_nine = ColorSpaces::power_of_2<std::size_t, 9>();
  auto two_at_ten = ColorSpaces::power_of_2<std::size_t, 10>();
  EXPECT_EQ(2 * two_at_nine, two_at_ten);
}


TEST(PowerOf2FloatingPointType, TwoAtZeroIsOne) {
  auto two_at_zero = ColorSpaces::power_of_2<double, 0>();
  EXPECT_EQ(1.0, two_at_zero);
}


TEST(PowerOf2FloatingPointType, TwoAtOneIsTwo) {
  auto two_at_one = ColorSpaces::power_of_2<double, 1>();
  EXPECT_EQ(2.0, two_at_one);
}


TEST(PowerOf2FloatingPointType, TwoAtTwoIsFour) {
  auto two_at_two = ColorSpaces::power_of_2<double, 2>();
  EXPECT_EQ(4.0, two_at_two);
}


TEST(PowerOf2FloatingPointType, TwoAtThreeIsFour) {
  auto two_at_three = ColorSpaces::power_of_2<double, 3>();
  EXPECT_EQ(8.0, two_at_three);
}


TEST(PowerOf2FloatingPointType, TwoAtTenIsOneThousandAndTwentyFour) {
  auto two_at_ten = ColorSpaces::power_of_2<double, 10>();
  EXPECT_EQ(1024.0, two_at_ten);
}


TEST(PowerOf2FloatingPointType, TwoAtKPlusOneIsTwoTimesTwoAtK) {
  auto two_at_nine = ColorSpaces::power_of_2<double, 9>();
  auto two_at_ten = ColorSpaces::power_of_2<double, 10>();
  EXPECT_EQ(2.0 * two_at_nine, two_at_ten);
}


TEST(Normalize, MinimumValueGoesToZeroUI8T) {
  uint8_t value = 0;
  auto normalized = ColorSpaces::normalize01(value);
  EXPECT_EQ(0.0, normalized);
}


TEST(Normalize, MinimumValueGoesToZeroUI16T) {
  uint16_t value = 0;
  auto normalized = ColorSpaces::normalize01<uint16_t, 8>(value);
  EXPECT_EQ(0.0, normalized);
}


TEST(Normalize, MaximumValueGoesToOneUI16T) {
  uint16_t value = 255;
  auto normalized = ColorSpaces::normalize01<uint16_t, 8>(value);
  EXPECT_EQ(1.0, normalized);
}


TEST(Normalize, AboveMaximumValueGoesToOneUI16T) {
  uint16_t value = 256;
  auto normalized = ColorSpaces::normalize01<uint16_t, 8>(value);
  EXPECT_EQ(1.0, normalized);
}


TEST(InverseNormalize, ZeroGoesToZeroUI8T) {
  double value = 0.0;
  auto inverse_normalized = ColorSpaces::inverse_normalize01(value);
  EXPECT_EQ(0, inverse_normalized);
}


TEST(InverseNormalize, ZeroGoesToZeroUI16T) {
  double value = 0.0;
  auto inverse_normalized =
      ColorSpaces::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(0, inverse_normalized);
}


TEST(InverseNormalize, BelowZeroGoesToZeroUI16T) {
  double value = -0.1;
  auto inverse_normalized =
      ColorSpaces::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(0, inverse_normalized);
}


TEST(InverseNormalize, OneGoesToMaximumUI16T) {
  double value = 1.0;
  auto inverse_normalized =
      ColorSpaces::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(255, inverse_normalized);
}


TEST(InverseNormalize, AboveOneGoesToMaximumValueUI16T) {
  double value = 1.25;
  auto inverse_normalized =
      ColorSpaces::inverse_normalize01<uint16_t, 8>(value);
  EXPECT_EQ(255, inverse_normalized);
}


// struct BT601ToRGBConversion : testing::Test {	
// };
template <typename T>
std::tuple<T,T,T> get_rgb_black() {
	return std::make_tuple<T, T, T>(0, 0, 0);
}

template <typename T, std::size_t bpp>
std::tuple<T,T,T> get_rgb_white() {
	T max = ColorSpaces::get_max_value_for_bpp<T, bpp>();
	return std::make_tuple(max, max, max);
}

template <typename T, std::size_t bpp>
std::tuple<T,T,T> get_rgb_red() {
	T max = ColorSpaces::get_max_value_for_bpp<T, bpp>();
	return std::make_tuple(max, 0, 0);
}


TEST(RGBToYCbCrBT601, BlackMapsToBlackLuminanceAndHalfChrominances) {
	auto rgb = get_rgb_black<uint16_t>();
	auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<uint16_t, 10, ColorSpaces::BT601Coefficients, true>(rgb);
	EXPECT_EQ(y, 0);
	EXPECT_EQ(cb, 512);
	EXPECT_EQ(cr, 512);
}

TEST(RGBToYCbCrBT601, WhiteMapsToMaxLuminanceAndHalfChrominances) {
	auto rgb = get_rgb_white<uint16_t, 10>();
	auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<uint16_t, 10, ColorSpaces::BT601Coefficients, true>(rgb);
	auto max = ColorSpaces::get_max_value_for_bpp<uint16_t, 10>();
	EXPECT_EQ(y, max);
	EXPECT_EQ(cb, 512);
	EXPECT_EQ(cr, 512);
}

TEST(RGBToYCbCrBT601, RedMapsToMaxCr) {
	auto rgb = get_rgb_red<uint16_t, 10>();
	auto ycbcr = ColorSpaces::convert_rgb_to_ycbcr<uint16_t, 10, ColorSpaces::BT601Coefficients, true>(rgb);
	auto cr = std::get<2>(ycbcr);
	auto max = ColorSpaces::get_max_value_for_bpp<uint16_t, 10>();
	EXPECT_EQ(cr, max);
}

template <typename T>
class RGBRedToYCbCrType : public ::testing::Test {
protected:
  std::tuple<T,T,T> rgb = get_rgb_red<T, 10>();
};

template <typename T>
class RGBWhiteToYCbCrType : public ::testing::Test {
protected:
  std::tuple<T,T,T> rgb = get_rgb_white<T, 8>();
};

using testing::Types;

typedef Types<uint16_t, uint32_t, uint64_t> unsigned_types_to_test;

struct bpp8{
	uint16_t bpp = 8;
};

struct bpp9{
	uint16_t bpp = 9;
};

struct bpp10{
	uint16_t bpp = 10;
};

using my_test_types = ::testing::Types<
	std::pair<uint16_t, bpp8>, 
	std::pair<uint16_t, bpp9>, 
	std::pair<uint16_t, bpp10>>;
	

TYPED_TEST_SUITE(RGBRedToYCbCrType, unsigned_types_to_test);
TYPED_TEST_SUITE(RGBWhiteToYCbCrType, unsigned_types_to_test);

TYPED_TEST(RGBRedToYCbCrType, RedMapsToMaxCr) {
	auto ycbcr = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 10, ColorSpaces::BT601Coefficients, true>(this->rgb);
	auto cr = std::get<2>(ycbcr);
	auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 10>();
	EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBWhiteToYCbCrType, RedMapsToMaxCr) {
	auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 8, ColorSpaces::BT601Coefficients, true>(this->rgb);
	auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 8>();
	auto half = (max+1)/2;
	EXPECT_EQ(y, max);
	EXPECT_EQ(cb, half);
	EXPECT_EQ(cr, half);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}