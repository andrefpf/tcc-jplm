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

#include <tuple>  //std::tie
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Utils/Stream/BinaryTools.h"
#include <variant>
#include <vector>

class VariableFloatingPointCoordinates {
 protected:
  uint8_t pp;

 public:
  VariableFloatingPointCoordinates(uint8_t pp) : pp(pp) {
  }

  auto get_pp() const noexcept {
    return pp;
  }
};


template<typename T>
class FloatingPointCoordinates : VariableFloatingPointCoordinates {
 protected:
  std::tuple<T, T, T> origin_position;  //x, y, z
  std::tuple<T, T, T> rotation_around_axis;
  std::tuple<T, T, T> scaling;

 public:
  FloatingPointCoordinates(const std::tuple<T, T, T>& origin_position,
      const std::tuple<T, T, T>& rotation_around_axis,
      const std::tuple<T, T, T>& scaling) : VariableFloatingPointCoordinates(sizeof(T)/2) {
    static_assert(std::numeric_limits<T>::is_iec559, "The coordinate type must be IEC559/IEEE 754");
  }

  
  constexpr uint64_t size() const noexcept {
    return (pp * 2) * 9;
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
  std::unique_ptr<VariableFloatingPointCoordinates> coordinates;
  //uint16_t extInt; //(is this necessary?)
  float baseline_x;
  float baseline_y;
  using calibration = std::variant<float, std::vector<float>>;
  std::array<calibration, 12> values;


 public:
  //! \todo implement this class (CalibrationContents)
  CalibrationContents() = default;
  virtual ~CalibrationContents() = default;


  virtual CalibrationContents* clone() const override {
    return new CalibrationContents(*this);
  }


  template<CalibrationParam calibration_type> 
  float get(uint64_t linear_position) const {
    const auto& value = std::get<calibration_type>(values);
    if(std::holds_alternative<float>(value)) {
      return std::get<0>(value);
    }
    return std::get<1>(value).at(linear_position);
  }


  uint64_t size() const noexcept override {
    return 0;  //
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