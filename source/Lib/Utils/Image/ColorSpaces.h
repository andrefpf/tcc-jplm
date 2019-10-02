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


#include <iostream>
#include <limits>
#include <memory>
#include <tuple>
#include <vector>
#include "Lib/Utils/Image/ColorModelUtils.h"


namespace ColorSpaces {

enum ColorSpace { RGB, BT601, BT709, BT2020, YCoCg };

enum RGB { R = 0, G = 1, B = 2 };
enum YCbCr { Y = 0, Cb = 1, Cr = 2 };
enum YCoCg { Yy, Co, Cg };  //cannot reuse the name Y :(
enum ColorCoefs { kb, kr };

//! \todo Change the implementation to have less errors
// class BT601Coefficients {
//  public:
//   static constexpr double kb = 0.114;
//   static constexpr double kr = 0.299;
//   static constexpr double kg = 1.0 - kb - kr;
// };

// class BT709Coefficients {
//  public:
//   static constexpr double kb = 0.0722;
//   static constexpr double kr = 0.2126;
//   static constexpr double kg = 1.0 - kb - kr;
// };

// class BT2020Coefficients {
//  public:
//   static constexpr double kb = 0.0593;
//   static constexpr double kr = 0.2627;
//   static constexpr double kg = 1.0 - kb - kr;
// };


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


template<typename T, std::size_t nbits>
constexpr void check_for_integral_type_with_at_least_8_bits() {
  static_assert(nbits >= 8, "The nbits must be larger or equal to 8.");
  static_assert(std::is_integral<T>::value, "Must be an integral type");
}


template<typename T = uint8_t, std::size_t nbits = 8>
T y_double_to_integral(double y) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 4>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 219.0;

  return static_cast<T>(std::round(mult_term * y + sum_term));
}


template<typename T = uint8_t, std::size_t nbits = 8>
double y_integral_to_double(T y) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 4>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 219.0;

  return (y - sum_term) / mult_term;
}


template<typename T = uint8_t, std::size_t nbits = 8>
T cbcr_double_to_integral(double cbcr) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  constexpr auto mult_term =
      static_cast<double>(power_of_2<T, nbits - 8>()) * 224.0;

  return static_cast<T>(std::round(mult_term * cbcr + sum_term));
}


template<typename T = uint8_t, std::size_t nbits = 8>
double cbcr_integral_to_double(T cbcr) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto sum_term = power_of_2<double, nbits - 1>();
  constexpr auto mult_term = power_of_2<double, nbits - 8>() * 224.0;

  return (cbcr - sum_term) / mult_term;
}


template<typename T = uint8_t, std::size_t nbits = 8>
double ycbcr_integral_to_double_no_dynamic_range_reduction(T value) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto mult_term = power_of_2<double, nbits>() - 1.0;


  return static_cast<double>(value) / mult_term;
}


template<typename T = uint8_t, std::size_t nbits = 8>
double y_integral_to_double_no_dynamic_range_reduction(T y) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto mult_term = power_of_2<double, nbits>() - 1.0;


  return static_cast<double>(y) / mult_term;
}


template<typename T = uint8_t, std::size_t nbits = 8>
T y_double_to_integral_no_dynamic_range_reduction(double y) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto mult_term = power_of_2<double, nbits>() - 1.0;


  return static_cast<T>(std::round(y * mult_term));
}


template<typename T = uint8_t, std::size_t nbits = 8>
double cbcr_integral_to_double_no_dynamic_range_reduction(T cbcr) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto sum_term = power_of_2<double, nbits - 1>();  //offset
  constexpr auto mult_term = power_of_2<double, nbits>() - 1.0;

  return static_cast<double>(cbcr - sum_term) / mult_term;
}


template<typename T = uint8_t, std::size_t nbits = 8>
T cbcr_double_to_integral_no_dynamic_range_reduction(double cbcr) {
  check_for_integral_type_with_at_least_8_bits<T, nbits>();
  using namespace ColorModelUtils;

  constexpr auto sum_term = power_of_2<double, nbits - 1>();  //offset
  constexpr auto mult_term = power_of_2<double, nbits>() - 1.0;

  return static_cast<T>(std::round(cbcr * mult_term) + sum_term);
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

  double y = kr * get<R>(rgb) + kg * get<G>(rgb) + kb * get<B>(rgb);
  double cb = (get<B>(rgb) - y) / kcb;
  double cr = (get<R>(rgb) - y) / kcr;

  return std::make_tuple(y, cb, cr);
}


template<typename T = uint8_t, std::size_t nbits = 8,
    typename ConversionCoefficients>
std::tuple<T, T, T> rgb_to_ycbcr_integral(const std::tuple<T, T, T>& rgb) {
  using namespace ColorModelUtils;
  static_assert(std::is_integral<T>::value,
      "This conversion function expects an integral typename");
  using namespace std;
  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  constexpr double kg = ConversionCoefficients::kg;
  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  const auto& [r, g, b] = rgb;

  double y = kr * r + kg * g + kb * b;
  double cb = (b - y) / kcb;
  double cr = (r - y) / kcr;

  constexpr auto shift_val = static_cast<double>(power_of_2<T, nbits - 1>());
  constexpr auto max_luminance =
      static_cast<T>(get_max_value_for_bpp<T, nbits>());
  constexpr auto max_val_color = static_cast<T>(shift_val) - 1;
  auto integral_y =
      static_cast<T>(clip_max<double, max_luminance>(std::round(y)));
  auto integral_cb = static_cast<T>(
      clip_max<double, max_val_color>(std::round(cb)) + shift_val);
  auto integral_cr = static_cast<T>(
      clip_max<double, max_val_color>(std::round(cr)) + shift_val);

  return {integral_y, integral_cb, integral_cr};
}


template<typename ConversionCoefficients>
std::tuple<double, double, double> ycbcr_to_rgb_base_double(
    const std::tuple<double, double, double>& ycbcr) {
  using namespace ColorModelUtils;
  using namespace std;

  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  constexpr double kg = ConversionCoefficients::kg;

  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  double b = get<Cb>(ycbcr) * kcb + get<Y>(ycbcr);  //
  double r = get<Cr>(ycbcr) * kcr + get<Y>(ycbcr);
  double g = (get<Y>(ycbcr) - kr * (r) -kb * (b)) / (kg);

  return std::make_tuple(r, g, b);
}


template<typename T = uint8_t, std::size_t nbits = 8,
    typename ConversionCoefficients>
std::tuple<T, T, T> ycbcr_to_rgb_integral(const std::tuple<T, T, T>& ycbcr) {
  static_assert(std::is_integral<T>::value,
      "This conversion function expects an integral typename");
  using namespace std;
  using namespace ColorModelUtils;

  constexpr double kb = ConversionCoefficients::kb;
  constexpr double kr = ConversionCoefficients::kr;
  // constexpr double kg = (1.0-kr-kb);
  constexpr double kg = ConversionCoefficients::kg;

  constexpr double kcb = 2.0 * (1.0 - ConversionCoefficients::kb);
  constexpr double kcr = 2.0 * (1.0 - ConversionCoefficients::kr);

  auto [y, cb, cr] = ycbcr;

  constexpr auto shift_val = power_of_2<double, nbits - 1>();
  double double_y = static_cast<double>(y);
  double double_cb = static_cast<double>(cb) - shift_val;
  double double_cr = static_cast<double>(cr) - shift_val;

  const double b = double_cb * kcb + double_y;  //
  const double r = double_cr * kcr + double_y;
  const double g = (double_y - kr * (r) -kb * (b)) / (kg);

  constexpr auto max_component =
      static_cast<T>(get_max_value_for_bpp<T, nbits>());

  const auto integral_r =
      static_cast<T>(clip_min_max<double, max_component>(std::round(r)));
  const auto integral_g =
      static_cast<T>(clip_min_max<double, max_component>(std::round(g)));
  const auto integral_b =
      static_cast<T>(clip_min_max<double, max_component>(std::round(b)));

  return {integral_r, integral_g, integral_b};
}


template<typename T, std::size_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
std::tuple<T, T, T> convert_rgb_to_ycbcr(const std::tuple<T, T, T>& rgb) {
  if constexpr (std::is_integral<T>::value && keep_dynamic_range &&
                (sizeof(T) * 8 > nbits)) {
    return rgb_to_ycbcr_integral<T, nbits, ConversionCoefficients>(rgb);
  }
  using namespace ColorModelUtils;

  auto normalized_rgb = std::make_tuple(normalize01<T, nbits>(std::get<R>(rgb)),
      normalize01<T, nbits>(std::get<G>(rgb)),
      normalize01<T, nbits>(std::get<B>(rgb)));
  auto ycbcr = rgb_to_ycbcr_base_double<ConversionCoefficients>(normalized_rgb);


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
  if constexpr ((std::is_integral<T>::value) && (sizeof(T) * 8 > nbits)) {
    return ycbcr_to_rgb_integral<T, nbits, ConversionCoefficients>(ycbcr);
  }
  using namespace ColorModelUtils;
  const auto [r, g, b] = ycbcr_to_rgb_base_double<ConversionCoefficients>(
      {y_integral_to_double_no_dynamic_range_reduction<T, nbits>(
           std::get<Y>(ycbcr)),
          cbcr_integral_to_double_no_dynamic_range_reduction<T, nbits>(
              std::get<Cb>(ycbcr)),
          cbcr_integral_to_double_no_dynamic_range_reduction<T, nbits>(
              std::get<Cr>(ycbcr))});
  return std::make_tuple(inverse_normalize01<T, nbits>(r),
      inverse_normalize01<T, nbits>(g), inverse_normalize01<T, nbits>(b));
}


template<typename T, std::size_t nbits, typename ConversionCoefficients>
std::tuple<T, T, T> convert_ycbcr_to_rgb_reducing_dynamic_range(
    const std::tuple<T, T, T>& ycbcr) {
  using namespace ColorModelUtils;
  const auto [r, g, b] = ycbcr_to_rgb_base_double<ConversionCoefficients>(
      {y_integral_to_double<T, nbits>(std::get<Y>(ycbcr)),
          cbcr_integral_to_double<T, nbits>(std::get<Cb>(ycbcr)),
          cbcr_integral_to_double<T, nbits>(std::get<Cr>(ycbcr))});
  return std::make_tuple(inverse_normalize01<T, nbits>(r),
      inverse_normalize01<T, nbits>(g), inverse_normalize01<T, nbits>(b));
}


template<typename T, std::size_t nbits, typename ConversionCoefficients,
    bool keep_dynamic_range>
std::tuple<T, T, T> convert_ycbcr_to_rgb(const std::tuple<T, T, T>& ycbcr) {
  if constexpr (keep_dynamic_range) {
    return convert_ycbcr_to_rgb_keeping_dynamic_range<T, nbits,
        ConversionCoefficients>(ycbcr);
  } else {
    return convert_ycbcr_to_rgb_reducing_dynamic_range<T, nbits,
        ConversionCoefficients>(ycbcr);
  }
}


template<typename T, std::size_t nbits>
std::tuple<T, T, T> convert_rgb_to_ycocg_round(const std::tuple<T, T, T>& rgb) {
  using namespace ColorModelUtils;
  const auto& [r, g, b] = rgb;
  auto temp_co = r - b;  //may be negative
  auto t = b + (temp_co >> 1);
  auto temp_cg = g - t;  //may be negative
  T y = t + (temp_cg >> 1);
  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  T co = temp_co + sum_term;
  T cg = temp_cg + sum_term;
  return {y, co, cg};
}


template<typename T, std::size_t nbits>
std::tuple<T, T, T> convert_ycocg_to_rgb_round(
    const std::tuple<T, T, T>& ycocg) {
  using namespace ColorModelUtils;
  const auto& [y, co, cg] = ycocg;
  constexpr auto sum_term = static_cast<double>(power_of_2<T, nbits - 1>());
  auto temp_co = co - sum_term;
  auto temp_cg = cg - sum_term;
  auto t = y - (temp_co >> 1);
  T g = temp_cg + t;
  T b = t - (temp_co >> 1);
  T r = temp_co + b;
  return {r, g, b};
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


template<typename T, typename ConversionCoefficients, bool keep_dynamic_range>
class ConversorProvider {
 public:
  ConversorProvider() = default;
  ~ConversorProvider() = default;


  std::unique_ptr<GenericColorSpacesConverter<T, ConversionCoefficients,
      keep_dynamic_range>>
  getConverter(std::size_t nbits) {
    switch (nbits) {
      case 8:
        return std::make_unique<ColorSpacesConverter<T, 8,
            ConversionCoefficients, keep_dynamic_range>>();
      case 9: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 9,
            ConversionCoefficients, keep_dynamic_range>>(); }
            [[fallthrough]];
      case 10: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 10,
            ConversionCoefficients, keep_dynamic_range>>(); }
            [[fallthrough]];
      case 11: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 11,
            ConversionCoefficients, keep_dynamic_range>>(); }
            [[fallthrough]];
      case 12: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 12,
            ConversionCoefficients, keep_dynamic_range>>(); } 
            [[fallthrough]];
      case 13: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 13,
            ConversionCoefficients, keep_dynamic_range>>(); } 
            [[fallthrough]];
      case 14: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 14,
            ConversionCoefficients, keep_dynamic_range>>(); }
            [[fallthrough]];
      case 15: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 15,
            ConversionCoefficients, keep_dynamic_range>>(); }
            [[fallthrough]];
      case 16: if constexpr (sizeof(T) > 1) {
        return std::make_unique<ColorSpacesConverter<T, 16,
            ConversionCoefficients, keep_dynamic_range>>(); }
            [[fallthrough]];
    }
    //this is the default
    std::cerr << "nbits=" << nbits
              << " is not supported (the supported ones are 8, 9, 10, 11, 12, "
                 "13, 14 and 15)."
              << std::endl;
    std::cerr << "Assuming 15 bits" << std::endl;
    if constexpr (sizeof(T) > 1) {
    return std::make_unique<ColorSpacesConverter<T, 15, ConversionCoefficients,
        keep_dynamic_range>>();
      }
  return nullptr;
  }
};

}  // namespace ColorSpaces

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_COLORSPACES_H__ */