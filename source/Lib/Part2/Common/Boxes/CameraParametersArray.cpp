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

/** \file     CameraParametersArray.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-24
 */

#include "Lib/Part2/Common/Boxes/CameraParametersArray.h"
#include <cstdint>


std::size_t count_ones(uint16_t ext_int) {
  std::size_t ones = 0;
  for (auto i = 0; i < 12; ++i) {
    if (ext_int & 0x1) {
      ones++;
    }
    ext_int >>= 1;
  }
  return ones;
}


CameraParametersArray::CameraParametersArray(
    const std::tuple<float, float> &baseline,
    const lightfield_dimension_type &rows,
    const lightfield_dimension_type &columns,
    std::array<camera_parameter, 12> camera_parameters)
    : baseline(baseline), n_view_rows(rows), n_view_columns(columns),
      n_views(rows * columns), fully_initialized(true),
      camera_parameters(camera_parameters) {
  //checking if camera parameters are correctly initialized
  for (const auto &param : camera_parameters) {
    if (param.index() == 1) {
      const auto &vec = std::get<1>(param);
      if (vec.size() != n_views) {
        throw CameraParameterBoxExceptions::
            InvalidCameraParameterArrayVectorSizeException(n_views, vec.size());
      }
    }
  }
}


CameraParametersArray::CameraParametersArray(
    const std::tuple<float, float> &baseline, const std::size_t &n_views,
    const std::array<camera_parameter, 12> &camera_parameters)
    : baseline(baseline), n_views(n_views), fully_initialized(false),
      camera_parameters(camera_parameters) {
  //checking if camera parameters are correctly initialized
  for (const auto &param : camera_parameters) {
    if (param.index() == 1) {
      const auto &vec = std::get<1>(param);
      if (vec.size() != n_views) {
        throw CameraParameterBoxExceptions::
            InvalidCameraParameterArrayVectorSizeException(n_views, vec.size());
      }
    }
  }
}


CameraParametersArray::CameraParametersArray(
    const std::tuple<float, float> &baseline, const std::size_t &n_views,
    std::array<camera_parameter, 12> &&camera_parameters)
    : baseline(baseline), n_views(n_views), fully_initialized(false),
      camera_parameters(std::move(camera_parameters)) {
  //checking if camera parameters are correctly initialized
  for (const auto &param : camera_parameters) {
    if (param.index() == 1) {
      const auto &vec = std::get<1>(param);
      if (vec.size() != n_views) {
        throw CameraParameterBoxExceptions::
            InvalidCameraParameterArrayVectorSizeException(n_views, vec.size());
      }
    }
  }
}


CameraParametersArray::CameraParametersArray(
    const std::tuple<float, float> &baseline, lightfield_dimension_type rows,
    lightfield_dimension_type columns, uint16_t ext_int)
    : baseline(baseline), n_view_rows(rows), n_view_columns(columns),
      n_views(rows * columns), fully_initialized(true),
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


uint16_t CameraParametersArray::get_ext_int_bits() const noexcept {
  uint16_t ext_int = 0;
  uint8_t count = 0;
  for (const auto &param : camera_parameters) {
    ext_int |= (param.index() << count++);
  }
  assert(count == 12);
  return ext_int;
}


uint64_t CameraParametersArray::size() const noexcept {
  //initially is size in number of values (floats)
  uint64_t size = 2;  //initialized with 2 == baselines
  for (const auto &param : camera_parameters) {
    if (param.index() == 1) {
      size += std::get<1>(param).size();
    } else {
      size++;
    }
  }
  return (size * sizeof(float)) + sizeof(uint16_t);
}


void CameraParametersArray::initialize_missing_row_and_column(
    lightfield_dimension_type rows, lightfield_dimension_type columns) {
  std::size_t n_views_from_param = rows * columns;
  if (n_views != n_views_from_param) {
    throw CameraParameterBoxExceptions::InvalidLazzyInitializationException(
        rows, columns, n_views);
  }
  n_view_rows = rows;
  n_view_columns = columns;
  fully_initialized = true;
}


std::vector<std::byte> CameraParametersArray::get_bytes() const noexcept {
  auto bytes = std::vector<std::byte>();
  bytes.reserve(this->size());
  //bytes.push_back(this->get_ext_int_bits())
  BinaryTools::append_big_endian_bytes(bytes, this->get_ext_int_bits());
  BinaryTools::append_big_endian_bytes(bytes, this->get_baseline());
  for (const auto &param_variant : camera_parameters) {
    std::visit(
        [&bytes](const auto &param) {
          BinaryTools::append_big_endian_bytes(bytes, param);
        },
        param_variant);
  }
  return bytes;
}


void CameraParametersArray::set_baseline(
    std::tuple<float, float> &&new_baseline) noexcept {
  baseline = std::move(new_baseline);
}


void CameraParametersArray::set_baseline(
    const std::tuple<float, float> &new_baseline) noexcept {
  baseline = new_baseline;
}


float CameraParametersArray::get_baseline_y() const noexcept {
  return std::get<1>(baseline);
}


float CameraParametersArray::get_baseline_x() const noexcept {
  return std::get<0>(baseline);
}


std::tuple<float, float> CameraParametersArray::get_baseline() const noexcept {
  return baseline;
}
