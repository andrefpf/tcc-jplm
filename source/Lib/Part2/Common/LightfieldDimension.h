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

/** \file     LightfieldDimension.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-03
 */

#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDDIMENSION_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDDIMENSION_H__

#include <tuple>
#include "Lib/Part2/Common/LightfieldCoordinate.h"


template<typename T = std::size_t>
class LightfieldDimension : public LightfieldCoordinate<T> {
 protected:
  void check_for_zero_dimension(
      const std::tuple<T, T, T, T>& dimensions) const {
    const auto& [t, s, v, u] = dimensions;
    // std::cout << "LightfieldDimension:check_for_zero_dimension" << t << "," << s << "," << v << "," << u << std::endl;
    if ((t == 0) || (s == 0) || (v == 0) || (u == 0)) {
      throw LightfieldDimensionExceptions::InvalidZeroDimensionException();
    }
  }

 public:
  static_assert(std::is_unsigned<T>::value,
      "The dimensions must be defined using an unsigned type");


  LightfieldDimension(const T t, const T s, const T v, const T u)
      : LightfieldCoordinate<T>(t, s, v, u) {
    check_for_zero_dimension(this->dimensions);
  };


  LightfieldDimension(const std::tuple<T, T, T, T>& dimensions)
      : LightfieldCoordinate<T>(dimensions) {
    check_for_zero_dimension(dimensions);
  };


  LightfieldDimension(const LightfieldCoordinate<T>& other)
      : LightfieldCoordinate<T>(other) {
    check_for_zero_dimension(other.get());
  }


  template<typename OtherT>
  LightfieldDimension(const LightfieldCoordinate<OtherT>& other)
      : LightfieldCoordinate<T>(other) {
  }


  bool operator==(const LightfieldDimension<T>& other) const {
    return this->dimensions == other.dimensions;
  }


  bool operator!=(const LightfieldDimension<T>& other) const {
    return this->dimensions != other.dimensions;
  }


  std::size_t get_number_of_views_per_lightfield() const noexcept {
    return this->get_t() * this->get_s();
  }


  std::size_t get_number_of_pixels_per_view() const noexcept {
    return this->get_v() * this->get_u();
  }


  std::size_t get_number_of_pixels_per_lightfield() const noexcept {
    return get_number_of_pixels_per_view() *
           get_number_of_views_per_lightfield();
  }
};


namespace std {
template<typename T>
struct tuple_size<LightfieldDimension<T>>
    : std::integral_constant<std::size_t, 4> {};

template<std::size_t I, typename T>
struct tuple_element<I, LightfieldDimension<T>> {
  using type = T;
};


}  // namespace std

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDDIMENSION_H__ */