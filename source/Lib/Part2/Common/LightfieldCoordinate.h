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

/** \file     LightfieldCoordinate.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-06
 */

#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELDCOORDINATE_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELDCOORDINATE_H__

#include <iostream>
#include <tuple>
#include "Lib/Part2/Common/CommonExceptions.h"

template<typename T>
class LightfieldCoordinate {
 protected:
  std::tuple<T, T, T, T> dimensions;

 public:
  static_assert(std::is_integral<T>::value,
      "The type of the lightfield coordinates must be integral.");


  LightfieldCoordinate(const T t, const T s, const T v, const T u) noexcept
      : dimensions({t, s, v, u}) {
  }


  LightfieldCoordinate(const std::tuple<T, T, T, T>& dimensions) noexcept
      : dimensions(dimensions) {
  }


  LightfieldCoordinate(const LightfieldCoordinate<T>& other) noexcept
      : dimensions(other.dimensions) {
  }


  template<typename OtherT>
  LightfieldCoordinate(const LightfieldCoordinate<OtherT>& other)
      : dimensions(std::make_tuple(static_cast<OtherT>(other.get_t()),
            static_cast<OtherT>(other.get_s()),
            static_cast<OtherT>(other.get_v()),
            static_cast<OtherT>(other.get_u()))) {
    const auto& [t, s, v, u] = other.get();
    if (t > std::numeric_limits<T>::max() ||
        t < std::numeric_limits<T>::min()) {
      throw LightfieldCoordinateExceptions::OverflowException();
    }
    if (s > std::numeric_limits<T>::max() ||
        s < std::numeric_limits<T>::min()) {
      throw LightfieldCoordinateExceptions::OverflowException();
    }
    if (v > std::numeric_limits<T>::max() ||
        v < std::numeric_limits<T>::min()) {
      throw LightfieldCoordinateExceptions::OverflowException();
    }
    if (u > std::numeric_limits<T>::max() ||
        u < std::numeric_limits<T>::min()) {
      throw LightfieldCoordinateExceptions::OverflowException();
    }
  }


  virtual ~LightfieldCoordinate() = default;


  std::tuple<T, T, T, T> as_tuple() const noexcept {
    return dimensions;
  }


  bool operator==(const LightfieldCoordinate<T>& other) const noexcept {
    return this->dimensions == other.dimensions;
  }


  bool operator!=(const LightfieldCoordinate<T>& other) const noexcept {
    return this->dimensions != other.dimensions;
  }


  LightfieldCoordinate<T>& operator=(LightfieldCoordinate<T>&& other) {
    if (this != &other) {
      std::swap(dimensions, other.dimensions);
    }
    return *this;
  }


  LightfieldCoordinate<T>& operator=(const LightfieldCoordinate<T>& other) {
    if (this != &other) {
      this->dimensions = other.dimensions;
    }
    return *this;
  }


  LightfieldCoordinate<T> operator-(
      const LightfieldCoordinate<T>& other) const {
    if constexpr (std::is_unsigned<T>::value) {
      if ((this->get_t() < other.get_t()) || (this->get_s() < other.get_s()) ||
          (this->get_v() < other.get_v()) || (this->get_u() < other.get_u())) {
        throw LightfieldCoordinateExceptions::UnsignedUnderflowException();
      }
    }

    T t = this->get_t() - other.get_t();
    T s = this->get_s() - other.get_s();
    T v = this->get_v() - other.get_v();
    T u = this->get_u() - other.get_u();
    return LightfieldCoordinate<T>(t, s, v, u);
  }


  LightfieldCoordinate<T> operator+(
      const LightfieldCoordinate<T>& other) const {
    //checking for overflow
    auto max_val = std::numeric_limits<T>::max();
    auto max_other_t = max_val - this->get_t();
    auto max_other_s = max_val - this->get_s();
    auto max_other_v = max_val - this->get_v();
    auto max_other_u = max_val - this->get_u();
    if ((other.get_t() > max_other_t) || (other.get_s() > max_other_s) ||
        (other.get_v() > max_other_v) || (other.get_u() > max_other_u)) {
      throw LightfieldCoordinateExceptions::OverflowException();
    }


    T t = this->get_t() + other.get_t();
    T s = this->get_s() + other.get_s();
    T v = this->get_v() + other.get_v();
    T u = this->get_u() + other.get_u();
    return LightfieldCoordinate<T>(t, s, v, u);
  }


  LightfieldCoordinate<T> operator/(const T& divisor) {
    return LightfieldCoordinate<T>(this->get_t() / divisor,
        this->get_s() / divisor, this->get_v() / divisor,
        this->get_u() / divisor);
  }


  LightfieldCoordinate<T> hadamard_product(
      const LightfieldCoordinate<T>& other) const {
    return LightfieldCoordinate<T>(this->get_t() * other.get_t(),
        this->get_s() * other.get_s(), this->get_v() * other.get_v(),
        this->get_u() * other.get_u());
  }


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


  std::pair<T, T> get_t_and_s() const noexcept {
    return std::make_pair(get_t(), get_s());
  }


  std::pair<T, T> get_v_and_u() const noexcept {
    return std::make_pair(get_v(), get_u());
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


  void set(const std::tuple<T, T, T, T>& other) noexcept {
    this->dimensions = other;
  }


  void print() const {
    std::cout << get_t() << ", " << get_s() << ", " << get_v() << ", "
              << get_u() << std::endl;
  }
};


//the lines below are used to access lf coordinate as tuples,
//this also enables structured binding :)
namespace std {
template<typename T>
struct tuple_size<LightfieldCoordinate<T>>
    : std::integral_constant<std::size_t, 4> {};


template<std::size_t I, typename T>
struct tuple_element<I, LightfieldCoordinate<T>> {
  using type = T;
};
}  // namespace std

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELDCOORDINATE_H__ */