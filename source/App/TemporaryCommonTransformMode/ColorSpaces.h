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
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef COLORSPACES_H__
#define COLORSPACES_H__

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
#include <assert.h>
#include <memory>
#include "Lib/Part2/Common/TransformMode/Block4D.h"
#include "Lib/Part2/Common/TransformMode/LightField.h"

// From pg 6 of BT.709:
// The operator INT returns the value of 0 for fractional parts in the range of 0 to 0.4999... and +1 for fractional parts
// in the range of 0.5 to 0.9999..., i.e. it rounds up fractions above 0.5.

namespace ColorSpaces {

enum ColorSpace { RGB, BT601, BT709, BT2020, YCoCg };

enum RGB { R = 0, G = 1, B = 2 };
enum YCbCr { Y = 0, Cb = 1, Cr = 2 };
enum YCoCg { Yy, Co, Cg };  //cannot reuse the name Y :(
enum ColorCoefs { kb, kr };

class BT601Coefficients {
 public:
  static constexpr double kb = 0.114;
  static constexpr double kr = 0.299;
  static constexpr double kg = 0.587;
};

class BT709Coefficients {
 public:
  static constexpr double kb = 0.0722;
  static constexpr double kr = 0.2126;
  static constexpr double kg = 0.7152;
};

class BT2020Coefficients {
 public:
  static constexpr double kb = 0.0593;
  static constexpr double kr = 0.2627;
  static constexpr double kg = 0.678;
};

template<typename T, uint8_t exp>
constexpr T power_of_2() {
  //TODO: include here some kind of static_assert to check if the exponent (exp) is compatible with the chosen type (T)..
  T odd = 1;
  if constexpr (exp % 2 == 1) {
    odd = 2;
  }
  T half = 1;
  if constexpr (exp != 0) {
    half = power_of_2<T, exp / 2>();
  }
  return odd * half * half;
}


//TODO: see how to make the type of power_of_2 dependent on the type T.
//NOTE: one should not use T as type for power_of_2 since it may lack representativeness. For instance with the default template params
// power_of_2<uint8_t, 8> will return 0, since 256 is out of the range of uint8_t [0..255]
template<typename T = uint8_t, uint32_t nbits = 8>
double normalize01(const T& value) {
  return value / static_cast<double>(power_of_2<uint32_t, nbits>() - 1);
}

template<typename T = uint8_t, uint32_t nbits = 8>
T inverse_normalize01(double value) {
  return static_cast<T>(std::round(
      value * (static_cast<double>(power_of_2<uint32_t, nbits>()) - 1.0)));
}


double clip01d(double value);


template<typename T = uint8_t, uint32_t nbits = 8>
T y_double_to_integral(double y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 4>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 219.0;

  return static_cast<T>(std::round(mult_term * y + sum_term));
}

template<typename T = uint8_t, uint32_t nbits = 8>
double y_integral_to_double(T y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 4>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 219.0;

  return (y - sum_term) / mult_term;
}

template<typename T = uint8_t, uint32_t nbits = 8>
T cbcr_double_to_integral(double cbcr) {
  static_assert(nbits >= 8, "nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 224.0;

  return static_cast<T>(std::round(mult_term * cbcr + sum_term));
}

template<typename T = uint8_t, uint32_t nbits = 8>
double cbcr_integral_to_double(T cbcr) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 224.0;

  return (cbcr - sum_term) / mult_term;
}

template<typename T = uint8_t, uint32_t nbits = 8>
double ycbcr_integral_to_double_no_dynamic_range_reduction(T value) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);


  return static_cast<double>(value) / mult_term;
}

template<typename T = uint8_t, uint32_t nbits = 8>
double y_integral_to_double_no_dynamic_range_reduction(T y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);


  return static_cast<double>(y) / mult_term;
}

template<typename T = uint8_t, uint32_t nbits = 8>
T y_double_to_integral_no_dynamic_range_reduction(double y) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);

  return static_cast<T>(std::round(y * mult_term));
}


template<typename T = uint8_t, uint32_t nbits = 8>
double cbcr_integral_to_double_no_dynamic_range_reduction(T cbcr) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term =
      static_cast<double>(power_of_2<T, nbits - 1>());  //offset
  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);

  return static_cast<double>(cbcr - sum_term) / mult_term;
}

template<typename T = uint8_t, uint32_t nbits = 8>
T cbcr_double_to_integral_no_dynamic_range_reduction(double cbcr) {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");

  constexpr auto sum_term =
      static_cast<double>(power_of_2<T, nbits - 1>());  //offset
  constexpr auto mult_term = static_cast<double>(power_of_2<T, nbits>() - 1.0);

  return static_cast<T>(std::round(cbcr * mult_term + sum_term));
}


template<typename ConversionCoefficients>
std::tuple<double, double, double> rgb_to_ycbcr_base_double(
    const std::tuple<double, double, double>& rgb) {
  using namespace std;

  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  constexpr double kg = ConversionCoefficients::kg;

  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  // double pixel =  0.299 * get<R>(rgb) + 0.587 * get<G>(rgb) + 0.114 * get<B>(rgb);
  // // auto p = 0.2999d;
  // double pixel_casted =  static_cast<double>(0.299) * get<R>(rgb) + static_cast<double>(0.587) * get<G>(rgb) + static_cast<double>(0.114) * get<B>(rgb);


  double y = kr * get<R>(rgb) + kg * get<G>(rgb) + kb * get<B>(rgb);
  double cb = (get<B>(rgb) - y) / kcb;
  double cr = (get<R>(rgb) - y) / kcr;

  return std::make_tuple(y, cb, cr);
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

template<typename T = uint8_t, uint32_t nbits = 8,
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


template<typename T = uint8_t, uint32_t nbits = 8,
    typename ConversionCoefficients>
std::tuple<T, T, T> ycbcr_to_rgb_integral(const std::tuple<T, T, T>& ycbcr) {
  using namespace std;
  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  constexpr double kg = ConversionCoefficients::kg;
  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  constexpr auto shift_val = static_cast<double>(power_of_2<T, nbits - 1>());
  auto shifted_cb = static_cast<T>(get<Cb>(ycbcr) - shift_val);
  auto shifted_cr = static_cast<T>(get<Cr>(ycbcr) - shift_val);

  double b = shifted_cb * kcb + get<Y>(ycbcr);  //
  double r = shifted_cr * kcr + get<Y>(ycbcr);
  double g = (get<Y>(ycbcr) - kr * (r) -kb * (b)) / (kg);

  auto integral_r = static_cast<T>(std::round(r));  //should clip...
  auto integral_g = static_cast<T>(std::round(g));
  auto integral_b = static_cast<T>(std::round(b));

  return {integral_r, integral_g, integral_b};
}

template<typename T, uint32_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
std::tuple<T, T, T> convert_rgb_to_ycbcr(const std::tuple<T, T, T>& rgb) {
  if constexpr (std::is_integral<T>::value) {
    return rgb_to_ycbcr_integral<T, nbits, ConversionCoefficients>(rgb);
  }

  auto normalized_rgb = std::make_tuple(normalize01<T, nbits>(std::get<R>(rgb)),
      normalize01<T, nbits>(std::get<G>(rgb)),
      normalize01<T, nbits>(std::get<B>(rgb)));
  auto ycbcr = rgb_to_ycbcr_base_double<ConversionCoefficients>(normalized_rgb);
  if (keep_dynamic_range) {
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

template<typename T, uint32_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
std::tuple<T, T, T> convert_ycbcr_to_rgb(const std::tuple<T, T, T>& ycbcr) {
  std::tuple<double, double, double> normalized_ycbcr;  // = std::make_tuple

  if (keep_dynamic_range) {
    normalized_ycbcr = std::make_tuple(
        y_integral_to_double_no_dynamic_range_reduction<T, nbits>(
            std::get<Y>(ycbcr)),
        cbcr_integral_to_double_no_dynamic_range_reduction<T, nbits>(
            std::get<Cb>(ycbcr)),
        cbcr_integral_to_double_no_dynamic_range_reduction<T, nbits>(
            std::get<Cr>(ycbcr)));
  } else {
    normalized_ycbcr =
        std::make_tuple(y_integral_to_double<T, nbits>(std::get<Y>(ycbcr)),
            cbcr_integral_to_double<T, nbits>(std::get<Cb>(ycbcr)),
            cbcr_integral_to_double<T, nbits>(std::get<Cr>(ycbcr)));
  }

  auto rgb = ycbcr_to_rgb_base_double<ConversionCoefficients>(normalized_ycbcr);

  return std::make_tuple(
      inverse_normalize01<T, nbits>(clip01d(std::get<R>(rgb))),
      inverse_normalize01<T, nbits>(clip01d(std::get<G>(rgb))),
      inverse_normalize01<T, nbits>(clip01d(std::get<B>(rgb))));
}

template<typename T, typename ConversionCoefficients, bool keep_dynamic_range>
class GenericColorSpacesConverter {
 public:
  GenericColorSpacesConverter() = default;
  ~GenericColorSpacesConverter() = default;
  virtual std::tuple<T, T, T> to_RGB(std::tuple<T, T, T> YCbCr) = 0;
  virtual std::tuple<T, T, T> to_YCbCr(std::tuple<T, T, T> RGB) = 0;
};


template<typename T, uint32_t nbits, typename ConversionCoefficients,
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
  getConverter(uint32_t nbits) {
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


#endif /* end of include guard: COLORSPACES_H__ */
