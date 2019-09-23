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

/** \file     CameraParameterBoxParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-20
 */

#include "CameraParameterBoxParser.h"


template<typename T>
std::unique_ptr<VariablePrecisionFloatingPointCoordinates> get_fp_coordinates(
    BoxParserHelperBase& box_parser_helper) {
  const auto& origin_position = box_parser_helper.get_next<T, T, T>();
  const auto& rotation_around_axis = box_parser_helper.get_next<T, T, T>();
  const auto& scaling = box_parser_helper.get_next<T, T, T>();
  return std::make_unique<FloatingPointCoordinates<T>>(
      origin_position, rotation_around_axis, scaling);
}


// std::array<camera_parameter, 12> get_camera_parameters(
//     BoxParserHelperBase& box_parser_helper,
//     uint16_t ext_int){

	// auto param_0 = (ext_int & 1) ? camera_parameter(std::vector<float>(n_views, 0.0)) : camera_parameter(0.0);

    // camera_parameters({(ext_int & 1) ? camera_parameter(
    //                                                          std::vector<float>(
    //                                                              n_views, 0.0))
    //                                                    : camera_parameter(0.0),
    // ((ext_int >> 1) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 2) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 3) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 4) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 5) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 6) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 7) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 8) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 9) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                      : camera_parameter(0.0),
    // ((ext_int >> 10) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                       : camera_parameter(0.0),
    // ((ext_int >> 11) & 1) ? camera_parameter(std::vector<float>(n_views, 0.0))
    //                       : camera_parameter(0.0)})

// }

std::unique_ptr<Box> JPLMBoxParser::CameraParameterBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  std::unique_ptr<VariablePrecisionFloatingPointCoordinates> fp_coordinates;

  auto precision_of_coordinates = box_parser_helper.get_next<uint16_t>();  //pp
  if ((1 < precision_of_coordinates) || (precision_of_coordinates > 2)) {
    //throw Precision not supported
  }

  if (precision_of_coordinates == 1) {  //float
    fp_coordinates = get_fp_coordinates<float>(box_parser_helper);
  } else {
    assert(precision_of_coordinates == 2);
    fp_coordinates = get_fp_coordinates<double>(box_parser_helper);
  }

  auto ext_int = box_parser_helper.get_next<uint16_t>();  //ExtInt

  const auto baseline = box_parser_helper.get_next<float, float>();

  // std::array<camera_parameter, 12> camera_parameter;

  // auto cam_param_array = CameraParametersArray(
  //     {baseline_x, baseline_y}, rows, columns, camera_parameters);

  // return std::make_unique<CameraParameterContents>(
  //     fp_coordinates, cam_param_array);

  return nullptr;
}