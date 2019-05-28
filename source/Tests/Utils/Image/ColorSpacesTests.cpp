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
#include <random>
#include <tuple>
#include "ColorSpaces.h"
#include "Metrics.h"
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

template<typename T, std::size_t bpp>
std::tuple<T, T, T> get_rgb_blue() {
  T max = ColorSpaces::get_max_value_for_bpp<T, bpp>();
  return std::make_tuple(0, 0, max);
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
  std::tuple<T, T, T> white = get_rgb_white<T, 10>();
  std::tuple<T, T, T> red = get_rgb_red<T, 10>();
  std::tuple<T, T, T> blue = get_rgb_blue<T, 10>();
};


template<typename T>
class RGBWhiteToYCbCrType : public ::testing::Test {
 protected:
  std::tuple<T, T, T> white = get_rgb_white<T, 8>();
  std::tuple<T, T, T> red = get_rgb_red<T, 8>();
  std::tuple<T, T, T> blue = get_rgb_blue<T, 8>();
};


template<typename A, std::size_t b>
struct TestTypesColorConversion {
  typedef A firstType;
  static constexpr auto secondType = b;
};


template<typename T>
class RGBToYCbCrTypeAndBpp : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white =
      get_rgb_white<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> red =
      get_rgb_red<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> blue =
      get_rgb_blue<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> black = get_rgb_black<var_type>();
};

template<typename T>
class RGBToYCbCrTypeAndBppA : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white =
      get_rgb_white<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> red =
      get_rgb_red<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> blue =
      get_rgb_blue<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> black = get_rgb_black<var_type>();
};

template<typename T>
class RGBToYCbCrTypeAndBppB : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white =
      get_rgb_white<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> red =
      get_rgb_red<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> blue =
      get_rgb_blue<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> black = get_rgb_black<var_type>();
};

template<typename T>
class RGBToYCbCrTypeAndBppC : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white =
      get_rgb_white<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> red =
      get_rgb_red<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> blue =
      get_rgb_blue<var_type, T::secondType>();
  std::tuple<var_type, var_type, var_type> black = get_rgb_black<var_type>();
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


TYPED_TEST_SUITE(RGBToYCbCrTypeAndBpp, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(RGBToYCbCrTypeAndBppA, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    RGBToYCbCrTypeAndBppB, unsigned64_types_and_bpps_to_test_up_to32);


TYPED_TEST(RGBRedToYCbCrType, RedMapsToMaxCr) {
  auto ycbcr = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 10,
      ColorSpaces::BT601Coefficients, true>(this->red);
  auto cr = std::get<2>(ycbcr);
  auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 10>();
  EXPECT_EQ(cr, max);
}


TYPED_TEST(
    RGBWhiteToYCbCrType, WhiteMapsToMaxLuminanceAndHalfChrominancesBT601) {
  auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 8,
      ColorSpaces::BT601Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 8>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBToYCbCrTypeAndBpp, WhiteMapsToMaxLuminanceAndHalfChrominancesBT601) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppA, WhiteMapsToMaxLuminanceAndHalfChrominancesBT601) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppB, WhiteMapsToMaxLuminanceAndHalfChrominancesBT601) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBWhiteToYCbCrType, WhiteMapsToMaxLuminanceAndHalfChrominancesBT709) {
  auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 8,
      ColorSpaces::BT709Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 8>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBToYCbCrTypeAndBpp, WhiteMapsToMaxLuminanceAndHalfChrominancesBT709) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppA, WhiteMapsToMaxLuminanceAndHalfChrominancesBT709) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppB, WhiteMapsToMaxLuminanceAndHalfChrominancesBT709) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBWhiteToYCbCrType, WhiteMapsToMaxLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] = ColorSpaces::convert_rgb_to_ycbcr<TypeParam, 8,
      ColorSpaces::BT2020Coefficients, true>(this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<TypeParam, 8>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBToYCbCrTypeAndBpp, WhiteMapsToMaxLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppA, WhiteMapsToMaxLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppB, WhiteMapsToMaxLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->white);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, max);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


//in gcc < 8 the non use of values in a structured binding may give wunused-variable
// gcc 8 dows not warn if using [[maybe_unused]]
//http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2360

TYPED_TEST(RGBToYCbCrTypeAndBpp, RedMapsToMaxCrBT2020) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppA, RedMapsToMaxCrBT2020) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppB, RedMapsToMaxCrBT2020) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBpp, RedMapsToMaxCrBT601) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppA, RedMapsToMaxCrBT601) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppB, RedMapsToMaxCrBT601) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBpp, RedMapsToMaxCrBT709) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppA, RedMapsToMaxCrBT709) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppB, RedMapsToMaxCrBT709) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cr, max);
}


TYPED_TEST(RGBToYCbCrTypeAndBpp, BlueMapsToMaxCbBT2020) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppA, BlueMapsToMaxCbBT2020) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppB, BlueMapsToMaxCbBT2020) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBpp, BlueMapsToMaxCbBT601) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppA, BlueMapsToMaxCbBT601) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppB, BlueMapsToMaxCbBT601) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBpp, BlueMapsToMaxCbBT709) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppA, BlueMapsToMaxCbBT709) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}

TYPED_TEST(RGBToYCbCrTypeAndBppB, BlueMapsToMaxCbBT709) {
  [[maybe_unused]] const auto& [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  EXPECT_EQ(cb, max);
}


TYPED_TEST(
    RGBToYCbCrTypeAndBpp, BlackMapsToZeroLuminanceAndHalfChrominancesBT601) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppA, BlackMapsToZeroLuminanceAndHalfChrominancesBT601) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppB, BlackMapsToZeroLuminanceAndHalfChrominancesBT601) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBToYCbCrTypeAndBpp, BlackMapsToZeroLuminanceAndHalfChrominancesBT709) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppA, BlackMapsToZeroLuminanceAndHalfChrominancesBT709) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppB, BlackMapsToZeroLuminanceAndHalfChrominancesBT709) {
  auto [y, cb,
      cr] = ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
      TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


TYPED_TEST(
    RGBToYCbCrTypeAndBpp, BlackMapsToZeroLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppA, BlackMapsToZeroLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}

TYPED_TEST(
    RGBToYCbCrTypeAndBppB, BlackMapsToZeroLuminanceAndHalfChrominancesBT2020) {
  auto [y, cb, cr] =
      ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->black);
  auto max = ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
      TypeParam::secondType>();
  auto half = (max + 1) / 2;
  EXPECT_EQ(y, 0);
  EXPECT_EQ(cb, half);
  EXPECT_EQ(cr, half);
}


template<typename A, std::size_t b, typename Coefs>
struct TestTypesColorConversionFromYCbCrCoefs {
  typedef A firstType;
  static constexpr auto secondType = b;
  typedef Coefs thirdType;
};


template<typename T>
double get_pixel_mse(
    const std::tuple<T, T, T>& original, const std::tuple<T, T, T>& decoded) {
  auto error_red = static_cast<int64_t>(std::get<0>(original)) -
                   static_cast<int64_t>(std::get<0>(decoded));
  auto error_green = static_cast<int64_t>(std::get<1>(original)) -
                     static_cast<int64_t>(std::get<1>(decoded));
  auto error_blue = static_cast<int64_t>(std::get<2>(original)) -
                    static_cast<int64_t>(std::get<2>(decoded));

  auto errors =
      std::vector<decltype(error_red)>({error_red, error_green, error_blue});
  return Metrics::get_mean_squared_error(errors);
}

template<typename T, std::size_t bpp, typename ConversionCoefficients>
std::tuple<T, T, T> get_ycbcr_black() {
  auto rgb_black = get_rgb_black<T>();
  return ColorSpaces::convert_rgb_to_ycbcr<T, bpp, ConversionCoefficients,
      true>(rgb_black);
}


template<typename T, std::size_t bpp, typename ConversionCoefficients>
std::tuple<T, T, T> get_ycbcr_white() {
  auto rgb_white = get_rgb_white<T, bpp>();
  return ColorSpaces::convert_rgb_to_ycbcr<T, bpp, ConversionCoefficients,
      true>(rgb_white);
}


template<typename T, std::size_t bpp, typename ConversionCoefficients>
std::tuple<T, T, T> get_ycbcr_red() {
  auto rgb_red = get_rgb_red<T, bpp>();
  return ColorSpaces::convert_rgb_to_ycbcr<T, bpp, ConversionCoefficients,
      true>(rgb_red);
}

template<typename T, std::size_t bpp, typename ConversionCoefficients>
std::tuple<T, T, T> get_ycbcr_blue() {
  auto rgb_blue = get_rgb_blue<T, bpp>();
  return ColorSpaces::convert_rgb_to_ycbcr<T, bpp, ConversionCoefficients,
      true>(rgb_blue);
}


template<typename T>
class YCbCrBT601ToRGBTypeAndBpp : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> blue =
      get_ycbcr_blue<var_type, T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT601Coefficients>();
};

template<typename T>
class YCbCrBT709ToRGBTypeAndBpp : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> blue =
      get_ycbcr_blue<var_type, T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT709Coefficients>();
};

template<typename T>
class YCbCrBT2020ToRGBTypeAndBpp : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> blue = get_ycbcr_blue<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
};

template<typename T>
class YCbCrBT601ToRGBTypeAndBpp32T : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> blue =
      get_ycbcr_blue<var_type, T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT601Coefficients>();
};

template<typename T>
class YCbCrBT709ToRGBTypeAndBpp32T : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> blue =
      get_ycbcr_blue<var_type, T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT709Coefficients>();
};

template<typename T>
class YCbCrBT2020ToRGBTypeAndBpp32T : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> blue = get_ycbcr_blue<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
};

template<typename T>
class YCbCrBT601ToRGBTypeAndBpp64T : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> blue =
      get_ycbcr_blue<var_type, T::secondType, ColorSpaces::BT601Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT601Coefficients>();
};

template<typename T>
class YCbCrBT709ToRGBTypeAndBpp64T : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> blue =
      get_ycbcr_blue<var_type, T::secondType, ColorSpaces::BT709Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT709Coefficients>();
};

template<typename T>
class YCbCrBT2020ToRGBTypeAndBpp64T : public ::testing::Test {
 protected:
  typedef typename T::firstType var_type;
  std::tuple<var_type, var_type, var_type> white = get_ycbcr_white<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> red =
      get_ycbcr_red<var_type, T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> blue = get_ycbcr_blue<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
  std::tuple<var_type, var_type, var_type> black = get_ycbcr_black<var_type,
      T::secondType, ColorSpaces::BT2020Coefficients>();
};

// bool tuple_is_equal
TYPED_TEST_SUITE(YCbCrBT601ToRGBTypeAndBpp, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(YCbCrBT709ToRGBTypeAndBpp, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(YCbCrBT2020ToRGBTypeAndBpp, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT601ToRGBTypeAndBpp32T, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT709ToRGBTypeAndBpp32T, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT2020ToRGBTypeAndBpp32T, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT601ToRGBTypeAndBpp64T, unsigned64_types_and_bpps_to_test_up_to32);
TYPED_TEST_SUITE(
    YCbCrBT709ToRGBTypeAndBpp64T, unsigned64_types_and_bpps_to_test_up_to32);
TYPED_TEST_SUITE(
    YCbCrBT2020ToRGBTypeAndBpp64T, unsigned64_types_and_bpps_to_test_up_to32);


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}

TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp32T, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp32T, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp32T, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->red);
  auto mse = get_pixel_mse(rgb_red_reference, converted_red);
  EXPECT_TRUE(mse <= 1.0);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp32T, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp32T, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp32T, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp32T, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp32T, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp32T, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp32T, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp32T, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  auto mse = get_pixel_mse(rgb_red_reference, converted_red);
  EXPECT_TRUE(mse <= 1.0);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp32T, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp64T, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp64T, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp64T, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBpp64T, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp64T, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp64T, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp64T, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBpp64T, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp64T, BlackConvertsBackToBlack) {
  auto rgb_black_reference = get_rgb_black<typename TypeParam::firstType>();
  auto converted_black =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->black);
  EXPECT_EQ(rgb_black_reference, converted_black);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp64T, WhiteConvertsBackToWhite) {
  auto rgb_white_reference =
      get_rgb_white<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_white =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->white);
  EXPECT_EQ(rgb_white_reference, converted_white);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp64T, RedConvertsBackToRed) {
  auto rgb_red_reference =
      get_rgb_red<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_red =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->red);
  EXPECT_TRUE(get_pixel_mse(rgb_red_reference, converted_red) <= 1.0);
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBpp64T, BlueConvertsBackToBlue) {
  auto rgb_blue_reference =
      get_rgb_blue<typename TypeParam::firstType, TypeParam::secondType>();
  auto converted_blue =
      ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
          TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
          this->blue);
  EXPECT_TRUE(get_pixel_mse(rgb_blue_reference, converted_blue) <= 1.0);
}


template<typename T>
class YCbCrBT2020ToRGBTypeAndBppRandom16T : public ::testing::Test {};
template<typename T>
class YCbCrBT2020ToRGBTypeAndBppRandom32T : public ::testing::Test {};
template<typename T>
class YCbCrBT2020ToRGBTypeAndBppRandom64T : public ::testing::Test {};
TYPED_TEST_SUITE(
    YCbCrBT2020ToRGBTypeAndBppRandom16T, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT2020ToRGBTypeAndBppRandom32T, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(YCbCrBT2020ToRGBTypeAndBppRandom64T,
    unsigned64_types_and_bpps_to_test_up_to32);

TYPED_TEST(YCbCrBT2020ToRGBTypeAndBppRandom16T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
            ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBppRandom32T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
            ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


TYPED_TEST(YCbCrBT2020ToRGBTypeAndBppRandom64T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT2020Coefficients, true>(
            ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


template<typename T>
class YCbCrBT601ToRGBTypeAndBppRandom16T : public ::testing::Test {};
template<typename T>
class YCbCrBT601ToRGBTypeAndBppRandom32T : public ::testing::Test {};
template<typename T>
class YCbCrBT601ToRGBTypeAndBppRandom64T : public ::testing::Test {};
TYPED_TEST_SUITE(
    YCbCrBT601ToRGBTypeAndBppRandom16T, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT601ToRGBTypeAndBppRandom32T, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(YCbCrBT601ToRGBTypeAndBppRandom64T,
    unsigned64_types_and_bpps_to_test_up_to32);

TYPED_TEST(YCbCrBT601ToRGBTypeAndBppRandom16T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBppRandom32T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


TYPED_TEST(YCbCrBT601ToRGBTypeAndBppRandom64T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT601Coefficients, true>(ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


template<typename T>
class YCbCrBT709ToRGBTypeAndBppRandom16T : public ::testing::Test {};
template<typename T>
class YCbCrBT709ToRGBTypeAndBppRandom32T : public ::testing::Test {};
template<typename T>
class YCbCrBT709ToRGBTypeAndBppRandom64T : public ::testing::Test {};
TYPED_TEST_SUITE(
    YCbCrBT709ToRGBTypeAndBppRandom16T, unsigned16_types_and_bpps_to_test);
TYPED_TEST_SUITE(
    YCbCrBT709ToRGBTypeAndBppRandom32T, unsigned32_types_and_bpps_to_test);
TYPED_TEST_SUITE(YCbCrBT709ToRGBTypeAndBppRandom64T,
    unsigned64_types_and_bpps_to_test_up_to32);

TYPED_TEST(YCbCrBT709ToRGBTypeAndBppRandom16T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBppRandom32T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


TYPED_TEST(YCbCrBT709ToRGBTypeAndBppRandom64T,
    OneThousandRandomTestsRGBtoYCbCrToRGBGivesMaximumMSEOne) {
  std::default_random_engine generator(42);
  std::uniform_int_distribution<typename TypeParam::firstType> distribution(
      0, ColorSpaces::get_max_value_for_bpp<typename TypeParam::firstType,
             TypeParam::secondType>());

  for (auto i = 0; i < 1000; ++i) {
    auto r = distribution(generator);
    auto g = distribution(generator);
    auto b = distribution(generator);
    auto ycbcr =
        ColorSpaces::convert_rgb_to_ycbcr<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(
            {r, g, b});
    auto converted_back_rgb =
        ColorSpaces::convert_ycbcr_to_rgb<typename TypeParam::firstType,
            TypeParam::secondType, ColorSpaces::BT709Coefficients, true>(ycbcr);
    auto mse = get_pixel_mse({r, g, b}, converted_back_rgb);
    EXPECT_TRUE(mse <= 1.0);
  }
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
