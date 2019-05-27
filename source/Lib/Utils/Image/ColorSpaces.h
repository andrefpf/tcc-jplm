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

/** \file     ColorSpaces.h
 *  \brief    Color Space conversion class
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-02-19
 */

#ifndef JPLM_LIB_UTILS_IMAGE_COLORSPACES_H__
#define JPLM_LIB_UTILS_IMAGE_COLORSPACES_H__

/*
 * ColorSpaces.h
 *
 * Author: Ismael Seidel
 */

#include <cmath>
#include <iostream>
#include <limits>
#include <tuple>
#include <vector>
// #include <typeinfo>
#include <memory>

// From pg 6 of BT.709:
// The operator INT returns the value of 0 for fractional parts in the range of 0 to 0.4999... and +1 for fractional parts
// in the range of 0.5 to 0.9999..., i.e. it rounds up fractions above 0.5.

namespace ColorSpaces {

enum ColorSpace { RGB, BT601, BT709, BT2020, YCoCg };
// enum class RGB   { R=0, G =1, B =2};
// enum class YCbCr { Y=0, Cb=1, Cr=2};
// enum class YCoCg { Y=0, Co=1, Cg=2};

enum RGB { R = 0, G = 1, B = 2 };
enum YCbCr { Y = 0, Cb = 1, Cr = 2 };
enum YCoCg { Yy, Co, Cg };  //cannot reuse the name Y :(
enum ColorCoefs { kb, kr };

class BT601Coefficients {
 public:
  static constexpr double kb = 0.114;
  static constexpr double kr = 0.299;
  // static constexpr double kg = 0.587;
  static constexpr double kg = 1.0 - kb - kr;
};

class BT709Coefficients {
 public:
  static constexpr double kb = 0.0722;
  static constexpr double kr = 0.2126;
  // static constexpr double kg = 0.7152;
  static constexpr double kg = 1.0 - kb - kr;
};

class BT2020Coefficients {
 public:
  static constexpr double kb = 0.0593;
  static constexpr double kr = 0.2627;
  // static constexpr double kg = 0.678;
  static constexpr double kg = 1.0 - kb - kr;
};

template<typename T, std::size_t exp>
constexpr std::enable_if_t<std::is_floating_point<T>::value, T> power_of_2() {
  T odd = static_cast<T>(1);
  if constexpr (exp % 2 == 1) {
    odd = static_cast<T>(2);
  }
  T half = static_cast<T>(1);
  if constexpr (exp != 0) {
    half = power_of_2<T, exp / 2>();
  }
  return odd * half * half;
}

template<typename T, std::size_t exp>
constexpr std::enable_if_t<std::is_integral<T>::value, T> power_of_2() {
  constexpr std::size_t bits_per_byte = 8;
  static_assert((sizeof(T) * bits_per_byte) > exp,
      "Power of two result will be larger than the supported by the defined "
      "variable");
  if constexpr (exp == 0)
    return static_cast<T>(1);
  return static_cast<T>(1) << exp;
}

template<typename T, std::size_t bpp>
constexpr T get_max_value_for_bpp() {
  std::size_t max_plus_one = power_of_2<T, bpp>();
  --max_plus_one;
  return static_cast<T>(max_plus_one);
}

template<typename T = uint8_t, std::size_t nbits = 8>
double normalize01(const T& value) {
  static_assert(std::is_unsigned<T>::value,
      "Only unsigned values are supported in this normalization");
  constexpr auto power_of_two = power_of_2<std::size_t, nbits>();
  if (value >= power_of_two)
    return 1.0;
  return value / static_cast<double>(power_of_two - 1.0);
}

template<typename T = uint8_t, std::size_t nbits = 8>
T inverse_normalize01(double value) {
  static_assert(std::is_unsigned<T>::value,
      "Only unsigned values are supported in this normalization");
  if (value > 1.0)
    return static_cast<T>(power_of_2<std::size_t, nbits>() - 1);
  if (value < 0.0)
    return 0;
  return static_cast<T>(std::round(
      value * (static_cast<double>(power_of_2<std::size_t, nbits>()) - 1.0)));
}

double clip01d(double value);

template<typename T = uint8_t, std::size_t nbits = 8>
T y_double_to_integral(double y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 4>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 219.0;

  return static_cast<T>(std::round(mult_term * y + sum_term));
}

template<typename T = uint8_t, std::size_t nbits = 8>
double y_integral_to_double(T y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 4>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 219.0;

  return (y - sum_term) / mult_term;
}

template<typename T = uint8_t, std::size_t nbits = 8>
T cbcr_double_to_integral(double cbcr) {
  static_assert(nbits >= 8, "nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 224.0;

  return static_cast<T>(std::round(mult_term * cbcr + sum_term));
}

template<typename T = uint8_t, std::size_t nbits = 8>
double cbcr_integral_to_double(T cbcr) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 224.0;

  return (cbcr - sum_term) / mult_term;
}

template<typename T = uint8_t, std::size_t nbits = 8>
double ycbcr_integral_to_double_no_dynamic_range_reduction(T value) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);


  return static_cast<double>(value) / mult_term;
}

template<typename T = uint8_t, std::size_t nbits = 8>
double y_integral_to_double_no_dynamic_range_reduction(T y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);


  return static_cast<double>(y) / mult_term;
}

template<typename T = uint8_t, std::size_t nbits = 8>
T y_double_to_integral_no_dynamic_range_reduction(double y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);


  return static_cast<T>(std::round(y * mult_term));
}


template<typename T = uint8_t, std::size_t nbits = 8>
double cbcr_integral_to_double_no_dynamic_range_reduction(T cbcr) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term =
      static_cast<double>(power_of_2<T, nbits - 1>());  //offset
  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);

  return static_cast<double>(cbcr - sum_term) / mult_term;
}

template<typename T = uint8_t, std::size_t nbits = 8>
T cbcr_double_to_integral_no_dynamic_range_reduction(double cbcr) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = power_of_2<double, nbits - 1>();  //offset
  constexpr auto mult_term = power_of_2<double, nbits>() - 1.0;

  std::cout << "sum_term " << sum_term << std::endl;
  std::cout << "mult_term " << mult_term << std::endl;
  std::cout << "cbcr " << cbcr << std::endl;
  std::cout << "std::round(cbcr * mult_term) " << std::round(cbcr * mult_term) << std::endl;

  return static_cast<T>(std::round(cbcr * mult_term) + sum_term);
}


template<typename ConversionCoefficients>
std::tuple<double, double, double> rgb_to_ycbcr_base_double(
    const std::tuple<double, double, double>& rgb) {
  using namespace std;

  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  // constexpr double kg = (1.0-kr-kb);
  constexpr double kg = ConversionCoefficients::kg;

  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  double y = kr * get<R>(rgb) + kg * get<G>(rgb) + kb * get<B>(rgb);
  double cb = (get<B>(rgb) - y) / kcb;
  double cr = (get<R>(rgb) - y) / kcr;

  return std::make_tuple(y, cb, cr);
}


template<typename T = uint8_t, std::size_t nbits = 8,
    typename ConversionCoefficients>
std::tuple<T, T, T> rgb_to_ycbcr_integral(const std::tuple<T, T, T>& rgb) {
  using namespace std;
  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  constexpr double kg = ConversionCoefficients::kg;
  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  double y = kr * get<R>(rgb) + kg * get<G>(rgb) + kb * get<B>(rgb);
  double cb = (get<B>(rgb) - y) / kcb;
  double cr = (get<R>(rgb) - y) / kcr;

  // constexpr auto max_val = static_cast<double>(power_of_2<T, nbits>());
  constexpr auto shift_val = static_cast<double>(power_of_2<T, nbits - 1>());
  auto integral_y = static_cast<T>(std::round(y));  //should clip...
  auto integral_cb = static_cast<T>(std::round(cb) + shift_val);
  auto integral_cr = static_cast<T>(std::round(cr) + shift_val);

  return {integral_y, integral_cb, integral_cr};
}


template<typename ConversionCoefficients>
std::tuple<double, double, double> ycbcr_to_rgb_base_double(
    const std::tuple<double, double, double>& ycbcr) {
  using namespace std;

  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  // constexpr double kg = (1.0-kr-kb);
  constexpr double kg = ConversionCoefficients::kg;

  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  double b = get<Cb>(ycbcr) * kcb + get<Y>(ycbcr);  //
  double r = get<Cr>(ycbcr) * kcr + get<Y>(ycbcr);
  double g = (get<Y>(ycbcr) - kr * (r) -kb * (b)) / (kg);

  return std::make_tuple(r, g, b);
}

template<typename T, std::size_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
std::tuple<T, T, T> convert_rgb_to_ycbcr(const std::tuple<T, T, T>& rgb) {
  auto normalized_rgb = std::make_tuple(normalize01<T, nbits>(std::get<R>(rgb)),
      normalize01<T, nbits>(std::get<G>(rgb)),
      normalize01<T, nbits>(std::get<B>(rgb)));
  auto ycbcr = rgb_to_ycbcr_base_double<ConversionCoefficients>(normalized_rgb);
  if constexpr (std::is_integral<T>::value) {
    return rgb_to_ycbcr_integral<T, nbits, ConversionCoefficients>(rgb);
  }

  if constexpr (keep_dynamic_range) {
    return std::make_tuple(
        y_double_to_integral_no_dynamic_range_reduction<T, nbits>(
            std::get<Y>(ycbcr)),
        cbcr_double_to_integral_no_dynamic_range_reduction<T, nbits>(
            std::get<Cb>(ycbcr)),
        cbcr_double_to_integral_no_dynamic_range_reduction<T, nbits>(
            std::get<Cr>(ycbcr)));
  }

  return std::make_tuple(y_double_to_integral<T, nbits>(std::get<Y>(ycbcr)),
      cbcr_double_to_integral<T, nbits>(std::get<Cb>(ycbcr)),
      cbcr_double_to_integral<T, nbits>(std::get<Cr>(ycbcr)));
}


template<typename T, std::size_t nbits, typename ConversionCoefficients>
std::tuple<T, T, T> convert_ycbcr_to_rgb_keeping_dynamic_range(
    const std::tuple<T, T, T>& ycbcr) {
  const auto [r, g, b] = ycbcr_to_rgb_base_double<ConversionCoefficients>(
      y_integral_to_double_no_dynamic_range_reduction<T, nbits>(
          std::get<Y>(ycbcr)),
      cbcr_integral_to_double_no_dynamic_range_reduction<T, nbits>(
          std::get<Cb>(ycbcr)),
      cbcr_integral_to_double_no_dynamic_range_reduction<T, nbits>(
          std::get<Cr>(ycbcr)));
  return std::make_tuple(inverse_normalize01<T, nbits>(r),
      inverse_normalize01<T, nbits>(g), inverse_normalize01<T, nbits>(b));
}

template<typename T, std::size_t nbits, typename ConversionCoefficients>
std::tuple<T, T, T> convert_ycbcr_to_rgb_reducing_dynamic_range(
    const std::tuple<T, T, T>& ycbcr) {
  const auto [r, g, b] = ycbcr_to_rgb_base_double<ConversionCoefficients>(
      y_integral_to_double<T, nbits>(std::get<Y>(ycbcr)),
      cbcr_integral_to_double<T, nbits>(std::get<Cb>(ycbcr)),
      cbcr_integral_to_double<T, nbits>(std::get<Cr>(ycbcr)));
  return std::make_tuple(inverse_normalize01<T, nbits>(r),
      inverse_normalize01<T, nbits>(g), inverse_normalize01<T, nbits>(b));
}


template<typename T, std::size_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
std::tuple<T, T, T> convert_ycbcr_to_rgb(const std::tuple<T, T, T>& ycbcr) {
  if constexpr (keep_dynamic_range) {
    return convert_ycbcr_to_rgb_keeping_dynamic_range(ycbcr);
  } else {
    return convert_ycbcr_to_rgb_reducing_dynamic_range(ycbcr);
  }
}


template<typename T, typename ConversionCoefficients, bool keep_dynamic_range>
class GenericColorSpacesConverter {
 public:
  GenericColorSpacesConverter() = default;
  ~GenericColorSpacesConverter() = default;
  virtual std::tuple<T, T, T> to_RGB(std::tuple<T, T, T> YCbCr) = 0;
  virtual std::tuple<T, T, T> to_YCbCr(std::tuple<T, T, T> RGB) = 0;
};


template<typename T, std::size_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
class ColorSpacesConverter : public GenericColorSpacesConverter<T,
                                 ConversionCoefficients, keep_dynamic_range> {
 public:
  ColorSpacesConverter() = default;
  ~ColorSpacesConverter() = default;
  virtual std::tuple<T, T, T> to_RGB(std::tuple<T, T, T> YCbCr) {
    return ColorSpaces::convert_ycbcr_to_rgb<T, nbits, ConversionCoefficients,
        keep_dynamic_range>(YCbCr);
  };
  virtual std::tuple<T, T, T> to_YCbCr(std::tuple<T, T, T> RGB) {
    return ColorSpaces::convert_rgb_to_ycbcr<T, nbits, ConversionCoefficients,
        keep_dynamic_range>(RGB);
  };
};

//TODO: instead of new, use unique_ptr
template<typename T, typename ConversionCoefficients, bool keep_dynamic_range>
class ConversorProvider {
 public:
  ConversorProvider() = default;
  ~ConversorProvider() = default;
  GenericColorSpacesConverter<T, ConversionCoefficients, keep_dynamic_range>*
  getConverter(std::size_t nbits) {
    switch (nbits) {
      case 8:
        return new ColorSpacesConverter<T, 8, ConversionCoefficients,
            keep_dynamic_range>();
      case 9:
        return new ColorSpacesConverter<T, 9, ConversionCoefficients,
            keep_dynamic_range>();
      case 10:
        return new ColorSpacesConverter<T, 10, ConversionCoefficients,
            keep_dynamic_range>();
      case 11:
        return new ColorSpacesConverter<T, 11, ConversionCoefficients,
            keep_dynamic_range>();
      case 12:
        return new ColorSpacesConverter<T, 12, ConversionCoefficients,
            keep_dynamic_range>();
      case 13:
        return new ColorSpacesConverter<T, 13, ConversionCoefficients,
            keep_dynamic_range>();
      case 14:
        return new ColorSpacesConverter<T, 14, ConversionCoefficients,
            keep_dynamic_range>();
      case 15:
        return new ColorSpacesConverter<T, 15, ConversionCoefficients,
            keep_dynamic_range>();
      case 16:
        return new ColorSpacesConverter<T, 16, ConversionCoefficients,
            keep_dynamic_range>();
    }
    //this is the default
    std::cerr << "nbits=" << nbits
              << " is not supported (the supported ones are 8, 9, 10, 11, 12, "
                 "13, 14, 15 and 16)."
              << std::endl;
    std::cerr << "Assuming 16 bits" << std::endl;
    return new ColorSpacesConverter<T, 16, ConversionCoefficients,
        keep_dynamic_range>();
  }
};

}  // namespace ColorSpaces

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_COLORSPACES_H__ */