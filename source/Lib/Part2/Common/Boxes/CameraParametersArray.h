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

/** \file     CameraParametersArray.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-20
 */

#ifndef JPLM_LIB_PART2_COMMON_BOXES_CAMERAPARAMETERSARRAY_H__
#define JPLM_LIB_PART2_COMMON_BOXES_CAMERAPARAMETERSARRAY_H__

#include <assert.h>
#include <array>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <tuple>  //std::tie
#include <variant>
#include <vector>
#include "Lib/Part2/Common/Boxes/CameraParameterType.h"
#include "Lib/Part2/Common/Boxes/CommonExceptions.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderContents.h"
#include "Lib/Utils/Stream/BinaryTools.h"


using camera_parameter = std::variant<float, std::vector<float>>;


std::size_t count_ones(uint16_t ext_int);


class CameraParametersArray {
 protected:
  /**
   * \brief The baseline (x,y) used to compute XCC and YCC parameters when they are not all defined
   */
  std::tuple<float, float> baseline;  //x, y
  /**
   * \warning Such value is not on the CameraParameterBox bitstream!
   */
  lightfield_dimension_type n_view_rows;
  /**
   * \warning Such value is not on the CameraParameterBox bitstream!
   */
  lightfield_dimension_type n_view_columns;
  /**
   * \warning Such value is not on the CameraParameterBox bitstream!
   */
  std::size_t n_views;
  /**
   * \brief indicates if the camera parameter array is fully initialized
   * \details By fully initialized we mean that there is enough data to 
   * \warning Such value is not on the CameraParameterBox bitstream!
   */
  bool fully_initialized;
  std::array<camera_parameter, 12> camera_parameters;

 public:
  CameraParametersArray(const std::tuple<float, float>& baseline,
      const lightfield_dimension_type& rows,
      const lightfield_dimension_type& columns,
      std::array<camera_parameter, 12> camera_parameters);


  /**
   * \brief      Lazzy constructs the object.
   * 
   * \details    The actual parameters can only be acessed after the number of rows and columns be set.
   *
   * \param[in]  baseline           The baseline
   * \param[in]  n_views            The total number of views (TxS)
   * \param[in]  camera_parameters  The camera parameters
   */
  CameraParametersArray(const std::tuple<float, float>& baseline,
      const std::size_t& n_views,
      const std::array<camera_parameter, 12>& camera_parameters);


  /**
   * \brief      Lazzy constructs the object.
   * 
   * \details    The actual parameters can only be acessed after the number of rows and columns be set.
   *
   * \param[in]  baseline           The baseline
   * \param[in]  n_views            The total number of views (TxS)
   * \param[in]  camera_parameters  The camera parameters
   */
  CameraParametersArray(const std::tuple<float, float>& baseline,
      const std::size_t& n_views,
      std::array<camera_parameter, 12>&& camera_parameters);


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
      uint16_t ext_int);


  /**
   * \brief      Gets the ExtInt bits as defined in Table A.8 of the standard
   *
   * \return     The extent int bits.
   */
  uint16_t get_ext_int_bits() const noexcept;


  /**
   * \brief      Gets the size (in bytes) of the data in this class 
   *
   * \return     The size (in bytes)
   */
  uint64_t size() const noexcept;


  /**
   * \brief      Finishes the initialization of this camera parameters array
   *
   * \param[in]  rows     The number of view rows in the light field
   * \param[in]  columns  The number of view columns in the light field
   */
  void initialize_missing_row_and_column(
      lightfield_dimension_type rows, lightfield_dimension_type columns);


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
          position) const;


  /**
   * \brief      Gets the baseline (x, y).
   *
   * \return     The baseline as a tuple ordered as baseline_x, baseline_y.
   */
  std::tuple<float, float> get_baseline() const noexcept;


  /**
   * \brief      Gets the baseline x.
   *
   * \return     The baseline x.
   */
  float get_baseline_x() const noexcept;


  /**
   * \brief      Gets the baseline y.
   *
   * \return     The baseline y.
   */
  float get_baseline_y() const noexcept;


  /**
   * \brief      Sets the baseline.
   *
   * \param[in]  new_baseline  The new baseline (expecting baseline_x, baseline_y tuple)
   *
   * \return     Returns the newly set baseline for further use
   */
  std::tuple<float, float> set_baseline(
      const std::tuple<float, float>& new_baseline) noexcept;


  /**
   * \brief      Sets the baseline (move version).
   *
   * \param      new_baseline  The new baseline
   *
   * \return     Returns the newly set baseline for further use
   */
  std::tuple<float, float> set_baseline(
      std::tuple<float, float>&& new_baseline) noexcept;


  std::vector<std::byte> get_bytes() const noexcept;
};


template<CameraParameterType camera_parameter_type>
float CameraParametersArray::get(
    const std::tuple<lightfield_dimension_type, lightfield_dimension_type>&
        position) const {
  if (!fully_initialized) {
    throw CameraParameterBoxExceptions::
        MissingCompleteInitializationException();
  }
  const auto& [t, s] = position;
  if ((t >= n_view_rows) || (s >= n_view_columns)) {
    throw CameraParameterBoxExceptions::InvalidCoordinateException(
        n_view_rows, n_view_columns, position);
  }
  const auto& value =
      std::get<static_cast<uint8_t>(camera_parameter_type)>(camera_parameters);
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
#endif /* end of include guard: JPLM_LIB_PART2_COMMON_BOXES_CAMERAPARAMETERSARRAY_H__ */