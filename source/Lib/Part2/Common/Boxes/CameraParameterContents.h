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

/** \file     CameraParameterContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_CAMERAPARAMETERCONTENTS_H__
#define JPLM_LIB_PART2_COMMON_BOXES_CAMERAPARAMETERCONTENTS_H__

#include <assert.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <ostream>
#include <tuple>  //std::tie
#include <variant>
#include <vector>
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Part2/Common/Boxes/CameraParameterType.h"
#include "Lib/Part2/Common/Boxes/CameraParametersArray.h"
#include "Lib/Part2/Common/Boxes/CommonExceptions.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderContents.h"
#include "Lib/Part2/Common/Boxes/VariablePrecisionFloatingPointCoordinates.h"
#include "Lib/Utils/Stream/BinaryTools.h"


template<typename T>
class FloatingPointCoordinates
    : public VariablePrecisionFloatingPointCoordinates {
 protected:
  /**
   * Tuple that contains \f$X_{LO}\f$, \f$Y_{LO}\f$ and \f$Z_{LO}\f$, respectively.
   */
  std::tuple<T, T, T> origin_position;  //x, y, z

  /**
   * Tuple that contains \f$\Theta_{XL}\f$, \f$\Theta_{YL}\f$ and \f$\Theta_{ZL}\f$, respectively.
   */
  std::tuple<T, T, T> rotation_around_axis;

  /**
   * Tuple that contains \f$S_{GLX}\f$, \f$S_{GLY}\f$ and \f$S_{GLZ}\f$, respectively.
   */
  std::tuple<T, T, T> scaling;
  constexpr uint64_t my_size() const noexcept {
    return sizeof(T) * 9 + 1;  //1 for pp
  }

  constexpr uint8_t compute_p() const {
    return std::log2(sizeof(T)) - 1;
  }

 public:
  virtual FloatingPointCoordinates<T>& test() {
    return *this;
  }

  FloatingPointCoordinates(const std::tuple<T, T, T>& origin_position,
      const std::tuple<T, T, T>& rotation_around_axis,
      const std::tuple<T, T, T>& scaling)
      : VariablePrecisionFloatingPointCoordinates(compute_p()),
        origin_position(origin_position),
        rotation_around_axis(rotation_around_axis), scaling(scaling) {
    static_assert(std::numeric_limits<T>::is_iec559,
        "The coordinate type must be IEC559/IEEE 754");
  }


  /**
   * \brief      Gets the size of the data (in bytes)
   * \details    There are 9 values. Thus, the total size in bytes depends on the data type 
                 of each value. 
   *
   * \return     The size (in bytes)
   */
  virtual uint64_t size() const noexcept override {
    return my_size();
  }


  /**
   * \brief      Gets the origin position.
   *
   * \return     The origin position tuple (x, y, z).
   * \note       It is possible to use structured binding
   */
  std::tuple<T, T, T> get_origin_position() const {
    return origin_position;
  }


  /**
   * \brief      Gets the rotation around axis angles.
   *
   * \return     The rotation around axis angle tuple (x, y, z).
   * \note       It is possible to use structured binding
   */
  std::tuple<T, T, T> get_rotation_around_axis() const {
    return rotation_around_axis;
  }


  /**
   * \brief      Gets the scaling for all axis.
   *
   * \return     The scaling tuple (x, y, z).
   * \note       It is possible to use structured binding
   */
  std::tuple<T, T, T> get_scaling() const {
    return scaling;
  }


  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     Copy of this object.
   */
  virtual FloatingPointCoordinates* clone() const override {
    return new FloatingPointCoordinates(*this);
  }


  // std::ostream& write_to(std::ostream& stream) const {
  //   stream << static_cast<const unsigned char>(this->get_pp());
  //    split_in_big_endian_bytes
  //   return stream;
  // }
  virtual std::vector<std::byte> get_bytes() const noexcept override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());

    bytes.emplace_back(std::byte{this->get_pp()});

    BinaryTools::append_big_endian_bytes(bytes, origin_position);
    BinaryTools::append_big_endian_bytes(bytes, rotation_around_axis);
    BinaryTools::append_big_endian_bytes(bytes, scaling);

    return bytes;
  }
};


class CameraParameterContents : public InMemoryDBox {
 protected:
  static_assert(std::numeric_limits<float>::is_iec559,
      "Float must be IEC559/IEEE754. However it seems that in this compiler "
      "this is not true.");
  static_assert(std::numeric_limits<double>::is_iec559,
      "Double must be IEC559/IEEE754. However it seems that in this compiler "
      "this is not true.");
  std::unique_ptr<VariablePrecisionFloatingPointCoordinates> coordinates;
  //uint16_t extInt; //(is this necessary?)
  CameraParametersArray camera_parameters;

 public:
  //! \todo implement this class (CalibrationConte  nts)
  CameraParameterContents(
      const VariablePrecisionFloatingPointCoordinates& coordinates,
      const CameraParametersArray& camera_parameters)
      : coordinates(std::unique_ptr<VariablePrecisionFloatingPointCoordinates>(
            coordinates.clone())),
        camera_parameters(camera_parameters) {
  }


  CameraParameterContents(
      std::unique_ptr<VariablePrecisionFloatingPointCoordinates>&& coordinates,
      std::tuple<float, float>&& baseline, lightfield_dimension_type rows,
      lightfield_dimension_type columns,
      std::array<camera_parameter, 12>&& camera_parameters)
      : coordinates(std::move(coordinates)),
        camera_parameters(CameraParametersArray(
            std::move(baseline), rows, columns, std::move(camera_parameters))) {
  }


  virtual ~CameraParameterContents() = default;


  CameraParameterContents(const CameraParameterContents& other)
      : coordinates(std::unique_ptr<VariablePrecisionFloatingPointCoordinates>(
            other.coordinates->clone())),
        camera_parameters(other.camera_parameters) {
  }


  CameraParameterContents(CameraParameterContents&& other)
      : coordinates(std::move(other.coordinates)),
        camera_parameters(std::move(other.camera_parameters)) {
  }


  virtual CameraParameterContents* clone() const override {
    return new CameraParameterContents(*this);
  }


  const VariablePrecisionFloatingPointCoordinates& get_ref_to_coordinates()
      const {
    return *coordinates;
  }


  VariablePrecisionFloatingPointCoordinates& get_ref_to_coordinates() {
    return *coordinates;
  }


  const CameraParametersArray& get_ref_to_camera_parameters() const {
    return camera_parameters;
  }


  CameraParametersArray& get_ref_to_camera_parameters() {
    return camera_parameters;
  }


  template<CameraParameterType camera_parameter_type>
  float get(
      const std::tuple<lightfield_dimension_type, lightfield_dimension_type>&
          position) const {
    return camera_parameters.get<camera_parameter_type>(position);
  }


  uint64_t size() const noexcept override {
    return coordinates->size() + camera_parameters.size();  //
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const CameraParameterContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const CameraParameterContents&) const noexcept {  //other
    return true;
  }


  bool operator!=(const CameraParameterContents& other) const noexcept {
    return !this->operator==(other);
  }


  virtual std::vector<std::byte> get_bytes() const override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    BinaryTools::byte_vector_cat(bytes, coordinates->get_bytes());
    BinaryTools::byte_vector_cat(bytes, camera_parameters.get_bytes());
    return bytes;
  }
};

// }  // namespace CameraParameters

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_CAMERAPARAMETERCONTENTS_H__ */

/*! \page camera_parameter_box_info The Camera Parameter Box
  \tableofcontents
  This page will introduce briefly how use the CameraParameterBox.
  \section Design
  The organization of the CameraParameterContents is a bit different from the standard specification.

  <table>	
  <tr><th> Field Name <th> Size (bits) <th> Value
  <tr><td> PP <td> 8 <td> 0 to 2^8-1
  </table>


  */