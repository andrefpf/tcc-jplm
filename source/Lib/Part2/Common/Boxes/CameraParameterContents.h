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
#include "Lib/Part2/Common/Boxes/CommonExceptions.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderContents.h"
#include "Lib/Utils/Stream/BinaryTools.h"


/**
 * \brief      Enumeration of Camera Parameter Types
 * \note    This enum is not part of the standard.
 *             It is used to access the various camera parameter types within the 
 *             CameraParameterBoxContents;
 */
enum CameraParameterType : uint8_t {
  XCC = 0,  //!< Camera centre along XL coordinate axis
  YCC = 1,  //!< Camera centre along YL coordinate axis
  ZCC = 2,  //!< Camera centre along ZL coordinate axis
  THETA_X_CAM = 3,  //!< Camera rotation offset along XL coordinate axis (rad)
  THETA_Y_CAM = 4,  //!< Camera rotation offset along YL coordinate axis (rad)
  THETA_Z_CAM = 5,  //!< Camera rotation offset along ZL coordinate axis (rad)
  F = 6,  //!< Focal lenght (mm)
  SW = 7,  //!< Sensor width (mm)
  SH = 8,  //!< Sensor width (mm)
  SK = 9,  //!< Sensor skew
  U0 = 10,  //!< Horizontal principle point offset
  V0 = 11  //!< Vertical principle point offset
};

// namespace CameraParameters {
using camera_parameter = std::variant<float, std::vector<float>>;

class CameraParametersArray {
 protected:
  /**
   * \brief The baseline (x,y) used to compute XCC and YCC parameters when they are not all defined
   */
  std::tuple<float, float> baseline;  //x, y
  /**
   * \warning Such value is on the CameraParameterBox bitstream!
   */
  lightfield_dimension_type n_view_rows;
  /**
   * \warning Such value is on the CameraParameterBox bitstream!
   */
  lightfield_dimension_type n_view_columns;
  /**
   * \warning Such value is on the CameraParameterBox bitstream!
   */
  std::size_t n_views;
  std::array<camera_parameter, 12> camera_parameters;

 public:
  CameraParametersArray(const std::tuple<float, float>& baseline,
      const lightfield_dimension_type& rows,
      const lightfield_dimension_type& columns,
      std::array<camera_parameter, 12> camera_parameters)
      : baseline(baseline), n_view_rows(rows), n_view_columns(columns),
        n_views(rows * columns), camera_parameters(camera_parameters) {
    //checking if camera parameters are correctly initialized
    for (const auto& param : camera_parameters) {
      if (param.index() == 1) {
        const auto& vec = std::get<1>(param);
        if (vec.size() != n_views) {
          throw CameraParameterBoxExceptions::
              InvalidCameraParameterArrayVectorSizeException(
                  n_views, vec.size());
        }
      }
    }
  }


  /**
   * \brief      Constructs the object.
   *
   * \param[in]  baseline  The baseline
   * \param[in]  rows      The number of rows (number of views in the vertical direction)
   * \param[in]  columns   The number of columns (number of views in the horizontal direction)
   * \param[in]  ext_int   The ExtInt bits (that will be used to instantiate a zeroed array of camera parameters)
   */
  CameraParametersArray(const std::tuple<float, float>& baseline,
      lightfield_dimension_type rows, lightfield_dimension_type columns,
      uint16_t ext_int)
      : baseline(baseline), n_view_rows(rows), n_view_columns(columns),
        n_views(rows * columns),
        camera_parameters(
            {(ext_int & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
                           : camera_parameter(0.0),
                ((ext_int >> 1) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 2) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 3) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 4) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 5) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 6) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 7) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 8) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 9) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 10) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0),
                ((ext_int >> 11) & 1)
                    ? camera_parameter(std::vector<float>(n_views, 0.0))
                    : camera_parameter(0.0)}) {
  }


  /**
   * \brief      Gets the ExtInt bits as defined in Table A.8 of the standard
   *
   * \return     The extent int bits.
   */
  uint16_t get_ext_int_bits() const noexcept {
    uint16_t ext_int = 0;
    uint8_t count = 0;
    for (const auto& param : camera_parameters) {
      ext_int |= (param.index() << count++);
    }
    assert(count == 12);
    return ext_int;
  }


  /**
   * \brief      Gets the size (in bytes) of the data in this class 
   *
   * \return     The size (in bytes)
   */
  uint64_t size() const noexcept {
    //initially is size in number of values (floats)
    uint64_t size = 2;  //initialized with 2 == baselines
    for (const auto& param : camera_parameters) {
      if (param.index() == 1) {
        size += std::get<1>(param).size();
      } else {
        size++;
      }
    }
    return (size * sizeof(float)) + sizeof(uint16_t);
  }


  /**
   * \brief      Compile time getter function for any available camera parameter 
   *
   * \param[in]  position               The position (t, s)
   *
   * \tparam     camera_parameter_type  One of the camera parameter types available, that are defined in CameraParameterType enum.
   *
   * \return     The requested camera parameter
   */
  template<CameraParameterType camera_parameter_type>
  float get(
      const std::tuple<lightfield_dimension_type, lightfield_dimension_type>&
          position) const {
    const auto& [t, s] = position;
    const auto& value = std::get<camera_parameter_type>(camera_parameters);
    if (std::holds_alternative<float>(value)) {
      if constexpr (camera_parameter_type == CameraParameterType::XCC) {
        return std::get<0>(value) + s * std::get<0>(baseline);
      }
      if constexpr (camera_parameter_type == CameraParameterType::YCC) {
        return std::get<0>(value) + t * std::get<1>(baseline);
      }
      return std::get<0>(value);
    }
    auto linear_position = t * n_view_columns + s;
    return std::get<1>(value).at(linear_position);
  }


  /**
   * \brief      Gets the baseline (x, y).
   *
   * \return     The baseline as a tuple ordered as baseline_x, baseline_y.
   */
  std::tuple<float, float> get_baseline() const noexcept {
    return baseline;
  }


  /**
   * \brief      Gets the baseline x.
   *
   * \return     The baseline x.
   */
  float get_baseline_x() const noexcept {
    return std::get<0>(baseline);
  }


  /**
   * \brief      Gets the baseline y.
   *
   * \return     The baseline y.
   */
  float get_baseline_y() const noexcept {
    return std::get<1>(baseline);
  }


  /**
   * \brief      Sets the baseline.
   *
   * \param[in]  new_baseline  The new baseline (expecting baseline_x, baseline_y tuple)
   *
   * \return     Returns the newly set baseline for further use
   */
  std::tuple<float, float> set_baseline(
      const std::tuple<float, float>& new_baseline) noexcept {
    baseline = new_baseline;
    return baseline;
  }


  /**
   * \brief      Sets the baseline (move version).
   *
   * \param      new_baseline  The new baseline
   *
   * \return     Returns the newly set baseline for further use
   */
  std::tuple<float, float> set_baseline(
      std::tuple<float, float>&& new_baseline) noexcept {
    baseline = std::move(new_baseline);
    return baseline;
  }


  std::vector<std::byte> get_bytes() const noexcept {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    //bytes.push_back(this->get_ext_int_bits())
    BinaryTools::append_big_endian_bytes(bytes, this->get_ext_int_bits());
    BinaryTools::append_big_endian_bytes(bytes, this->get_baseline());
    for (const auto& param_variant : camera_parameters) {
      std::visit(
          [&bytes](const auto& param) {
            BinaryTools::append_big_endian_bytes(bytes, param);
          },
          param_variant);
    }
    return bytes;
  }
};

/**
 * \brief      Base virtual class for variable precision floating point coordinates.
 */
class VariablePrecisionFloatingPointCoordinates {
 protected:
  //PP = 0, precision = 16 == "short float" //unavailable
  //PP = 1, precision = 32 == float
  //PP = 2, precision = 64 == double
  //PP = 3, precision = 128 == "long double" //unavailable (some implemetation uses 80 bits...)
  // ...
  uint8_t pp;  //!< Precision of coordinates

 public:
  /**
   * \brief      Constructs the object storing the precision
   *
   * \param[in]  pp    Precision. 
   *   <table>  
      <tr><th> PP value <th> Precision (in bits) <th> FP Type
      <tr><td> 0 <td> 16 <td> not available
      <tr><td> 1 <td> 32 <td> <b>float</b>
      <tr><td> 2 <td> 64 <td> <b>double</b>
      <tr><td> 3 <td> 128 <td> not ensured
      </table>
   * 
   */
  VariablePrecisionFloatingPointCoordinates(uint8_t pp) : pp(pp) {
  }

  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     Raw pointer to a copy of this object.
   */
  virtual VariablePrecisionFloatingPointCoordinates* clone() const = 0;


  /**
   * \brief      Gets the pp (precision).
   *
   * \return     The pp.
   */
  auto get_pp() const noexcept {
    return pp;
  }

  /**
   * \brief      Size of the contents (in bytes)
   *
   * \return     The size (in bytes)
   */
  virtual uint64_t size() const noexcept = 0;


  /**
   * \brief      Gets the bytes (std::byte) in the specified order within a std::vector.
   * \details    Each value must be converted to a big endian sequence of bytes.
   *
   * \return     The bytes (vector of bytes).
   */
  virtual std::vector<std::byte> get_bytes() const noexcept = 0;

  
  virtual VariablePrecisionFloatingPointCoordinates& test() = 0;
};


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
    return sizeof(T) * 9 +1; //1 for pp
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