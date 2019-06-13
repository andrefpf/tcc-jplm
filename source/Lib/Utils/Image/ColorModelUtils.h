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

/** \file     ColorModelUtils.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-13
 */

#ifndef JPLM_LIB_UTILS_IMAGE_COLORMODELUTILS_H__
#define JPLM_LIB_UTILS_IMAGE_COLORMODELUTILS_H__

#include <climits>  //for CHAR_BIT
#include <cmath>
#include <cstdint>
#include <type_traits>


namespace ColorModelUtils {

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
  constexpr std::size_t bits_per_byte = CHAR_BIT;  //number of bits in a byte
  static_assert((sizeof(T) * bits_per_byte) > exp,
      "Power of two result will be larger than the supported by the defined "
      "variable");
  if constexpr (exp == 0)
    return static_cast<T>(1);
  return static_cast<T>(1) << exp;
}


template<typename T, std::size_t bpp>
constexpr T get_max_value_for_bpp() {
  constexpr std::size_t max_plus_one = power_of_2<T, bpp>() - 1;
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

}  // namespace ColorModelUtils

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_COLORMODELUTILS_H__ */