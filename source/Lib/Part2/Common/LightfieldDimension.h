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

template<typename T>
class LightfieldDimension {
 protected:
  std::tuple<T, T, T, T> dimensions;

 public:
  static_assert(std::is_integral<T>::value,
      "The type of the dimensions must be integral.");
  static_assert(std::is_unsigned<T>::value,
      "The dimensions must be defined using an unsigned type");


  LightfieldDimension(const T t, const T s, const T v, const T u)
      : dimensions({t, s, v, u}){};


  LightfieldDimension(const std::tuple<T, T, T, T>& dimensions)
      : dimensions(dimensions){};


  ~LightfieldDimension() = default;


  T get_t() const noexcept {
    return std::get<0>(dimensions);
  }


  T get_s() const noexcept {
    return std::get<1>(dimensions);
  }


  T get_v() const noexcept {
    return std::get<2>(dimensions);
  }


  T get_u() const noexcept {
    return std::get<3>(dimensions);
  }


  auto get() const noexcept {
    return dimensions;
  }


  template<std::size_t N>
  auto get() const {  //for std::get<N>(dimension) access
    if constexpr (N == 0)
      return get_t();
    else if constexpr (N == 1)
      return get_s();
    else if constexpr (N == 2)
      return get_v();
    else if constexpr (N == 3)
      return get_u();
  }


  void set_t(const T t) noexcept {
    std::get<0>(dimensions) = t;
  }


  void set_s(const T s) noexcept {
    std::get<1>(dimensions) = s;
  }


  void set_v(const T v) noexcept {
    std::get<2>(dimensions) = v;
  }


  void set_u(const T u) noexcept {
    std::get<3>(dimensions) = u;
  }


  void set(const std::tuple<T, T, T, T>& other) {
    this->dimensions = other;
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