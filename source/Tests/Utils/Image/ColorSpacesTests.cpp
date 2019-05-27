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


template<typename T>
std::tuple<T, T, T> get_rgb_black() {
  return std::make_tuple<T, T, T>(0, 0, 0);
}


template<typename T, std::size_t bpp>
std::tuple<T, T, T> get_rgb_white() {
  T max = ColorSpaces::get_max_value_for_bpp<T, bpp>();
  return std::make_tuple(max, max, max);
}


template<typename T, std::size_t bpp>
std::tuple<T, T, T> get_rgb_red() {
  T max = ColorSpaces::get_max_value_for_bpp<T, bpp>();
  return std::make_tuple(max, 0, 0);
}


TEST(RGBToYCbCrBT601, BlackMapsToBlackLuminanceAndHalfChrominances) {
  auto rgb = get_rgb_black<uint16_t>();
  auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<uint16_t, 10,
      ColorSpaces::BT601Coefficients, true>(rgb);
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, 512);
  EXPECT_EQ(cr, 512);
}


TEST(RGBToYCbCrBT601, WhiteMapsToMaxLuminanceAndHalfChrominances) {
  auto rgb = get_rgb_white<uint16_t, 10>();
  auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<uint16_t, 10,
      ColorSpaces::BT601Coefficients, true>(rgb);
  auto max = ColorSpaces::get_max_value_for_bpp<uint16_t, 10>();
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, 512);
  EXPECT_EQ(cr, 512);
}


TEST(RGBToYCbCrBT601, RedMapsToMaxCr) {
  auto rgb = get_rgb_red<uint16_t, 10>();
  auto ycbcr = ColorSpaces::convert_rgb_to_ycbcr<uint16_t, 10,
      ColorSpaces::BT601Coefficients, true>(rgb);
  auto cr = std::get<2>(ycbcr);
  auto max = ColorSpaces::get_max_value_for_bpp<uint16_t, 10>();
  EXPECT_EQ(cr, max);
}


template<typename T>
class RGBRedToYCbCrType : public ::testing::Test {
 protected:
  std::tuple<T, T, T> rgb = get_rgb_red<T, 10>();
};


template<typename T>
class RGBWhiteToYCbCrType : public ::testing::Test {
 protected:
  std::tuple<T, T, T> rgb = get_rgb_white<T, 8>();
};


template<typename A, std::size_t b>
struct TestTypesColorConversion {
  typedef A firstType;
  static constexpr auto secondType = b;
};


template<typename T>
class RGBWhiteToYCbCrTypeAndBpp : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> rgb =
      get_rgb_white<var_type, T::secondType>();
};

template<typename T>
class RGBWhiteToYCbCrTypeAndBppA : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> rgb =
      get_rgb_white<var_type, T::secondType>();
};

template<typename T>
class RGBWhiteToYCbCrTypeAndBppB : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> rgb =
      get_rgb_white<var_type, T::secondType>();
};

template<typename T>
class RGBWhiteToYCbCrTypeAndBppC : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> rgb =
      get_rgb_white<var_type, T::secondType>();
};


using testing::Types;

typedef Types<TestTypesColorConversion<uint16_t, 8>,
    TestTypesColorConversion<uint16_t, 9>,
    TestTypesColorConversion<uint16_t, 10>,
    TestTypesColorConversion<uint16_t, 11>,
    TestTypesColorConversion<uint16_t, 12>,
    TestTypesColorConversion<uint16_t, 13>,
    TestTypesColorConversion<uint16_t, 14>,
    TestTypesColorConversion<uint16_t, 15>>
    unsigned16_types_and_bpps_to_test;

typedef Types<TestTypesColorConversion<uint32_t, 8>,
    TestTypesColorConversion<uint32_t, 9>,
    TestTypesColorConversion<uint32_t, 10>,
    TestTypesColorConversion<uint32_t, 11>,
    TestTypesColorConversion<uint32_t, 12>,
    TestTypesColorConversion<uint32_t, 13>,
    TestTypesColorConversion<uint32_t, 14>,
    TestTypesColorConversion<uint32_t, 15>,
    TestTypesColorConversion<uint32_t, 16>,
    TestTypesColorConversion<uint32_t, 17>,
    TestTypesColorConversion<uint32_t, 18>,
    TestTypesColorConversion<uint32_t, 19>,
    TestTypesColorConversion<uint32_t, 20>,
    TestTypesColorConversion<uint32_t, 21>,
    TestTypesColorConversion<uint32_t, 22>,
    TestTypesColorConversion<uint32_t, 23>,
    TestTypesColorConversion<uint32_t, 24>,
    TestTypesColorConversion<uint32_t, 25>,
    TestTypesColorConversion<uint32_t, 26>,
    TestTypesColorConversion<uint32_t, 27>,
    TestTypesColorConversion<uint32_t, 28>,
    TestTypesColorConversion<uint32_t, 29>,
    TestTypesColorConversion<uint32_t, 30>,
    TestTypesColorConversion<uint32_t, 31>>
    unsigned32_types_and_bpps_to_test;

typedef Types<TestTypesColorConversion<uint64_t, 8>,
    TestTypesColorConversion<uint64_t, 9>,
    TestTypesColorConversion<uint64_t, 10>,
    TestTypesColorConversion<uint64_t, 11>,
    TestTypesColorConversion<uint64_t, 12>,
    TestTypesColorConversion<uint64_t, 13>,
    TestTypesColorConversion<uint64_t, 14>,
    TestTypesColorConversion<uint64_t, 15>,
    TestTypesColorConversion<uint64_t, 16>,
    TestTypesColorConversion<uint64_t, 17>,
    TestTypesColorConversion<uint64_t, 18>,
    TestTypesColorConversion<uint64_t, 19>,
    TestTypesColorConversion<uint64_t, 20>,
    TestTypesColorConversion<uint64_t, 21>,
    TestTypesColorConversion<uint64_t, 22>,
    TestTypesColorConversion<uint64_t, 23>,
    TestTypesColorConversion<uint64_t, 24>,
    TestTypesColorConversion<uint64_t, 25>,
    TestTypesColorConversion<uint64_t, 26>,
    TestTypesColorConversion<uint64_t, 27>,
    TestTypesColorConversion<uint64_t, 28>,
    TestTypesColorConversion<uint64_t, 29>,
    TestTypesColorConversion<uint64_t, 30>,
    TestTypesColorConversion<uint64_t, 31>,
    TestTypesColorConversion<uint64_t, 32>>
    unsigned64_types_and_bpps_to_test_up_to32;


typedef Types<TestTypesColorConversion<uint64_t, 33>,
    TestTypesColorConversion<uint64_t, 34>,
    TestTypesColorConversion<uint64_t, 35>,
    TestTypesColorConversion<uint64_t, 36>,
    TestTypesColorConversion<uint64_t, 37>,
    TestTypesColorConversion<uint64_t, 38>,
    TestTypesColorConversion<uint64_t, 39>,
    TestTypesColorConversion<uint64_t, 40>,
    TestTypesColorConversion<uint64_t, 41>,
    TestTypesColorConversion<uint64_t, 42>,
    TestTypesColorConversion<uint64_t, 43>,
    TestTypesColorConversion<uint64_t, 44>,
    TestTypesColorConversion<uint64_t, 45>,
    TestTypesColorConversion<uint64_t, 46>,
    TestTypesColorConversion<uint64_t, 47>,
    TestTypesColorConversion<uint64_t, 48>,
    TestTypesColorConversion<uint64_t, 49>,
    TestTypesColorConversion<uint64_t, 50>,
    TestTypesColorConversion<uint64_t, 51>,
    TestTypesColorConversion<uint64_t, 52>,
    TestTypesColorConversion<uint64_t, 53>,
    TestTypesColorConversion<uint64_t, 54>,
    TestTypesColorConversion<uint64_t, 55>,
    TestTypesColorConversion<uint64_t, 56>,
    TestTypesColorConversion<uint64_t, 57>,
    TestTypesColorConversion<uint64_t, 58>,
    TestTypesColorConversion<uint64_t, 59>,
    TestTypesColorConversion<uint64_t, 60>,
    TestTypesColorConversion<uint64_t, 61>,
    TestTypesColorConversion<uint64_t, 62>,
    TestTypesColorConversion<uint64_t, 63>>
    unsigned64_types_and_bpps_to_test_up_to63;

typedef Types<uint16_t, uint32_t, uint64_t> unsigned_types_to_test;

// typedef Types<
// 	std::pair<uint16_t, 8>,
// 	std::pair<uint16_t, 9>,
// 	std::pair<uint16_t, 10>,
// 	std::pair<uint16_t, 11>,
// 	std::pair<uint16_t, 12>,
// 	std::pair<uint16_t, 13>,
// 	std::pair<uint16_t, 14>,
// 	std::pair<uint16_t, 15>
// 	> unsigned_types_to_test_2;

TYPED_TEST_SUITE(RGBRedToYCbCrType, unsigned_types_to_test);
TYPED_TEST_SUITE(RGBWhiteToYCbCrType, unsigned_types_to_test);


TYPED_TEST_SUITE(RGBWhiteToYCbCrTypeAndBpp, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(RGBWhiteToYCbCrTypeAndBppA, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    RGBWhiteToYCbCrTypeAndBppB, unsigned64_types_and_bpps_to_test_up_to32);
TYPED_TEST_SUITE(
    RGBWhiteToYCbCrTypeAndBppC, unsigned64_types_and_bpps_to_test_up_to63);


TYPED_TEST(RGBRedToYCbCrType, RedMapsToMaxCr) {
  auto ycbcr = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 10,
      ColorSpaces::BT601Coefficients, true>(this->rgb);
  auto cr = std::get<2>(ycbcr);
  auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 10>();
  EXPECT_EQ(cr, max);
}


TYPED_TEST(RGBWhiteToYCbCrType, RedMapsToMaxCr) {
  auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 8,
      ColorSpaces::BT601Coefficients, true>(this->rgb);
  auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 8>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(RGBWhiteToYCbCrTypeAndBpp, RedMapsToMaxCr) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->rgb);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(RGBWhiteToYCbCrTypeAndBppA, RedMapsToMaxCr) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->rgb);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(RGBWhiteToYCbCrTypeAndBppB, RedMapsToMaxCr) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->rgb);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(RGBWhiteToYCbCrTypeAndBppC, RedMapsToMaxCr) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->rgb);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
