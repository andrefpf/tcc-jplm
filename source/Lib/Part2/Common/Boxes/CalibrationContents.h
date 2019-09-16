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

/** \file     CalibrationContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_CALIBRATIONCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_BOXES_CALIBRATIONCONTENTS_H__

#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>
#include <tuple>  //std::tie
#include <variant>
#include <vector>
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Utils/Stream/BinaryTools.h"

class VariablePrecisionFloatingPointCoordinates {
 protected:
  //PP = 0, precision = 16 == "short float" //unavailable
  //PP = 1, precision = 32 == float
  //PP = 2, precision = 64 == double
  //PP = 3, precision = 128 == "long double" //unavailable (some implemetation uses 80 bits...)
  // ...
  uint8_t pp;  //!< Precision of coordinates

 public:
  VariablePrecisionFloatingPointCoordinates(uint8_t pp) : pp(pp) {
  }


  virtual VariablePrecisionFloatingPointCoordinates* clone() const = 0;

  auto get_pp() const noexcept {
    return pp;
  }


  virtual uint64_t size() const noexcept = 0;
};


template<typename T>
class FloatingPointCoordinates
    : public VariablePrecisionFloatingPointCoordinates {
 protected:
  std::tuple<T, T, T> origin_position;  //x, y, z
  std::tuple<T, T, T> rotation_around_axis;
  std::tuple<T, T, T> scaling;
  constexpr uint64_t my_size() const noexcept {
    return sizeof(T) * 9;
  }

  constexpr uint8_t compute_p() const {
    return std::log2(sizeof(T)) - 1;
  }

 public:
  FloatingPointCoordinates(const std::tuple<T, T, T>& origin_position,
      const std::tuple<T, T, T>& rotation_around_axis,
      const std::tuple<T, T, T>& scaling)
      : VariablePrecisionFloatingPointCoordinates(compute_p()),
        origin_position(origin_position),
        rotation_around_axis(rotation_around_axis), scaling(scaling) {
    static_assert(std::numeric_limits<T>::is_iec559,
        "The coordinate type must be IEC559/IEEE 754");
  }


  virtual uint64_t size() const noexcept override {
    return my_size();
  }


  std::tuple<T, T, T> get_origin_position() const {
    return origin_position;
  }


  std::tuple<T, T, T> get_rotation_around_axis() const {
    return rotation_around_axis;
  }


  std::tuple<T, T, T> get_scaling() const {
    return scaling;
  }

  virtual FloatingPointCoordinates* clone() const override {
    return new FloatingPointCoordinates(*this);
  }


  // std::ostream& write_to(std::ostream& stream) const {
  //   stream << static_cast<const unsigned char>(this->get_pp());
  //    split_in_big_endian_bytes
  //   return stream;
  // }
  std::vector<std::byte> get_bytes() const noexcept {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());

    bytes.emplace_back(std::byte{this->get_pp()});

    BinaryTools::append_big_endian_bytes(bytes, std::get<0>(origin_position));
    BinaryTools::append_big_endian_bytes(bytes, std::get<1>(origin_position));
    BinaryTools::append_big_endian_bytes(bytes, std::get<2>(origin_position));

    BinaryTools::append_big_endian_bytes(bytes, std::get<0>(rotation_around_axis));
    BinaryTools::append_big_endian_bytes(bytes, std::get<1>(rotation_around_axis));
    BinaryTools::append_big_endian_bytes(bytes, std::get<2>(rotation_around_axis));

    BinaryTools::append_big_endian_bytes(bytes, std::get<0>(scaling));
    BinaryTools::append_big_endian_bytes(bytes, std::get<1>(scaling));
    BinaryTools::append_big_endian_bytes(bytes, std::get<2>(scaling));
    
    return bytes;
  }
};


enum CalibrationParam : uint8_t {
  XCC = 0,
  YCC = 1,
  ZCC = 2,
  THETA_X_CAM = 3,
  THETA_Y_CAM = 4,
  THETA_Z_CAM = 5,
  F = 6,
  SW = 7,
  SH = 8,
  SK = 9,
  U0 = 10,
  V0 = 11
};


class CalibrationContents : public InMemoryDBox {
 protected:
  static_assert(std::numeric_limits<float>::is_iec559,
      "Float must be IEC559/IEEE754. However it seems that in this compiler "
      "this is not true.");
  static_assert(std::numeric_limits<double>::is_iec559,
      "Double must be IEC559/IEEE754. However it seems that in this compiler "
      "this is not true.");
  std::unique_ptr<VariablePrecisionFloatingPointCoordinates> coordinates;
  //uint16_t extInt; //(is this necessary?)
  std::tuple<float, float> baseline;  //x, y
  using calibration = std::variant<float, std::vector<float>>;
  std::array<calibration, 12> values;


 public:
  //! \todo implement this class (CalibrationContents)
  CalibrationContents(
      const VariablePrecisionFloatingPointCoordinates& coordinates,
      const std::tuple<float, float>& baseline,
      const std::array<calibration, 12>& values)
      : coordinates(std::unique_ptr<VariablePrecisionFloatingPointCoordinates>(
            coordinates.clone())),
        baseline(baseline), values(values) {
  }


  CalibrationContents(
      std::unique_ptr<VariablePrecisionFloatingPointCoordinates>&& coordinates,
      std::tuple<float, float>&& baseline, std::array<calibration, 12>&& values)
      : coordinates(std::move(coordinates)), baseline(std::move(baseline)),
        values(std::move(values)) {
  }


  virtual ~CalibrationContents() = default;


  CalibrationContents(const CalibrationContents& other)
      : coordinates(std::unique_ptr<VariablePrecisionFloatingPointCoordinates>(
            other.coordinates->clone())),
        baseline(other.baseline), values(other.values) {
  }


  virtual CalibrationContents* clone() const override {
    return new CalibrationContents(*this);
  }


  template<CalibrationParam calibration_type>
  float get(uint64_t linear_position) const {
    const auto& value = std::get<calibration_type>(values);
    if (std::holds_alternative<float>(value)) {
      return std::get<0>(value);
    }
    return std::get<1>(value).at(linear_position);
  }


  uint64_t size() const noexcept override {
    return coordinates->size();  //
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const CalibrationContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const CalibrationContents&) const noexcept {  //other
    return true;
  }


  bool operator!=(const CalibrationContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    return bytes;
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_CALIBRATIONCONTENTS_H__ */