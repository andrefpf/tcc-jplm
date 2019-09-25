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

/** \file     CameraParameterBoxTests.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-25
 */

#include <iostream>
#include "Lib/Part2/Common/Boxes/CameraParameterBox.h"
#include "gtest/gtest.h"
#include <fstream>      // std::ofstream
#include <filesystem>

template<typename T>
struct SimpleCameraParameterContentsTest : public testing::TestWithParam<T> {
 protected:
  std::tuple<T, T, T> xyz_coordinates;
  std::tuple<T, T, T> xyz_angles;
  std::tuple<T, T, T> xyz_scalings;
  constexpr static float baseline_x = 3.1415;  //dummy, just to check if io works
  constexpr static float baseline_y = 55.625;
  constexpr static float y00 = 115.0;
  constexpr static float z00 = 42.0;
  constexpr static float theta_x = 1.25;
  constexpr static float sk = 1;
  constexpr static float u0 = 0.111;
  constexpr static uint32_t rows = 3;  //3x4 lightfield
  constexpr static uint32_t columns = 4;  //3x4 lightfield
  constexpr static uint16_t ExtInt = 0x9F1;  //is 100111110001
  constexpr static float initial_value = 0.789101112;
  constexpr static float const_add_value = 0.042;
  std::vector<float> xcc_vec;
  std::vector<float> theta_y_vec;
  std::vector<float> theta_z_vec;
  std::vector<float> f_vec;
  std::vector<float> sW_vec;
  std::vector<float> sH_vec;
  std::vector<float> v0_vec;

  SimpleCameraParameterContentsTest()
      : xyz_coordinates({1.5, 2.25, 3.125}), xyz_angles({0.0, 3.1415, 3.125}),
        xyz_scalings({1.0, 0.5, 2.0}) {
    xcc_vec.resize(rows * columns);
    theta_y_vec.resize(rows * columns);
    theta_z_vec.resize(rows * columns);
    f_vec.resize(rows * columns);
    sW_vec.resize(rows * columns);
    sH_vec.resize(rows * columns);
    v0_vec.resize(rows * columns);

    auto value_source = initial_value;

    for (auto& value : xcc_vec) {
      value = value_source;
      value_source += const_add_value;
    }
    for (auto& value : theta_y_vec) {
      value = value_source;
      value_source += const_add_value;
    }
    for (auto& value : theta_z_vec) {
      value = value_source;
      value_source += const_add_value;
    }
    for (auto& value : f_vec) {
      value = value_source;
      value_source += const_add_value;
    }
    for (auto& value : sW_vec) {
      value = value_source;
      value_source += const_add_value;
    }
    for (auto& value : sH_vec) {
      value = value_source;
      value_source += const_add_value;
    }
    for (auto& value : v0_vec) {
      value = value_source;
      value_source += const_add_value;
    }
  };


  std::unique_ptr<CameraParameterContents> get_contents() {
    auto fp_coordinates =
        FloatingPointCoordinates<T>(xyz_coordinates, xyz_angles, xyz_scalings);
    std::array<camera_parameter, 12> camera_parameters;
    camera_parameters[0].emplace<std::vector<float>>(xcc_vec);
    camera_parameters[1].emplace<float>(y00);
    camera_parameters[2].emplace<float>(z00);
    camera_parameters[3].emplace<float>(theta_x);
    camera_parameters[4].emplace<std::vector<float>>(theta_y_vec);
    camera_parameters[5].emplace<std::vector<float>>(theta_z_vec);
    camera_parameters[6].emplace<std::vector<float>>(f_vec);
    camera_parameters[7].emplace<std::vector<float>>(sW_vec);
    camera_parameters[8].emplace<std::vector<float>>(sH_vec);
    camera_parameters[9].emplace<float>(sk);
    camera_parameters[10].emplace<float>(u0);
    camera_parameters[11].emplace<std::vector<float>>(v0_vec);

    auto cam_param_array = CameraParametersArray(
        {baseline_x, baseline_y}, rows, columns, camera_parameters);
    return std::make_unique<CameraParameterContents>(
        fp_coordinates, cam_param_array);
  }

  std::uint64_t expected_fp_coordinates_size() const {
    return 9 * sizeof(T) + 1;  // 9 coordinates + pp
  }


  std::uint64_t expected_camera_parameter_size() const {
    return 2 * sizeof(float) +  //baselines
           sizeof(uint16_t) +  //ExtInt
           5 * sizeof(float) +  // floats that appear only once
           7 * 12 * sizeof(float);
  }


  std::uint64_t expected_size() const {
    return expected_fp_coordinates_size() + expected_camera_parameter_size();
  }
};