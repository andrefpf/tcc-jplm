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
#include "Lib/Part2/Common/Boxes/FloatingPointCoordinates.h"
#include "Lib/Part2/Common/Boxes/LightFieldHeaderContents.h"
#include "Lib/Utils/Stream/BinaryTools.h"


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
      CameraParametersArray&& camera_parameters)
      : coordinates(std::move(coordinates)),
        camera_parameters(std::move(camera_parameters)) {
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


  virtual CameraParameterContents* clone() const override;


  const VariablePrecisionFloatingPointCoordinates& get_ref_to_coordinates()
      const;


  VariablePrecisionFloatingPointCoordinates& get_ref_to_coordinates();


  const CameraParametersArray& get_ref_to_camera_parameters() const;


  CameraParametersArray& get_ref_to_camera_parameters();


  template<CameraParameterType camera_parameter_type>
  float get(
      const std::tuple<lightfield_dimension_type, lightfield_dimension_type>&
          position) const;


  uint64_t size() const noexcept override;


  virtual bool is_equal(const DBox& other) const override;


  bool operator==(const CameraParameterContents&) const noexcept;


  bool operator!=(const CameraParameterContents& other) const noexcept;


  virtual std::vector<std::byte> get_bytes() const override;
};


template<CameraParameterType camera_parameter_type>
float CameraParameterContents::get(
    const std::tuple<lightfield_dimension_type, lightfield_dimension_type>&
        position) const {
  return camera_parameters.get<camera_parameter_type>(position);
}

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