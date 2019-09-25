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

/** \file     CameraParameterBoxParserTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-24
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part2/Decoder/Boxes/CameraParameterBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "Tests/Part2/Common/Boxes/CameraParameterBoxTests.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";


TEST(BasicTest, ReadsAllDataFromStream) {
  std::string filename(resources_path + "/boxes/camera_parameters_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto box = BoxParserRegistry::get_instance().parse<CameraParameterBox>(
      ManagedStream(if_stream, std::filesystem::file_size(filename)));

  EXPECT_EQ(if_stream.tellg(), std::filesystem::file_size(filename));
}


struct ParsingOfCameraParamAlone : public testing::Test {
 public:
  std::unique_ptr<CameraParameterBox> parse_box(const std::string& res_path) {
    auto filename = std::string(res_path + "/boxes/camera_parameters_box.bin");
    std::ifstream if_stream(filename, std::ifstream::binary);
    return BoxParserRegistry::get_instance().parse<CameraParameterBox>(
        ManagedStream(if_stream, std::filesystem::file_size(filename)));
  }
};


TEST_F(ParsingOfCameraParamAlone, GetsOriginPosition) {
  auto box = parse_box(resources_path);
  const auto& coordinates = box->get_ref_to_contents().get_ref_to_coordinates();
  const auto& cast_coordinates =
      static_cast<const FloatingPointCoordinates<float>&>(coordinates);

  const auto& [a, b, c] = cast_coordinates.get_origin_position();
  EXPECT_FLOAT_EQ(a, 1.5);
  EXPECT_FLOAT_EQ(b, 2.25);
  EXPECT_FLOAT_EQ(c, 3.125);
}


TEST_F(ParsingOfCameraParamAlone, GetsRotationAroundAxis) {
  auto box = parse_box(resources_path);
  const auto& coordinates = box->get_ref_to_contents().get_ref_to_coordinates();
  const auto& cast_coordinates =
      static_cast<const FloatingPointCoordinates<float>&>(coordinates);

  const auto& [a, b, c] = cast_coordinates.get_rotation_around_axis();
  EXPECT_FLOAT_EQ(a, 0.0);
  EXPECT_FLOAT_EQ(b, 3.1415);
  EXPECT_FLOAT_EQ(c, 3.125);
}


TEST_F(ParsingOfCameraParamAlone, GetsScaling) {
  auto box = parse_box(resources_path);
  const auto& coordinates = box->get_ref_to_contents().get_ref_to_coordinates();
  const auto& cast_coordinates =
      static_cast<const FloatingPointCoordinates<float>&>(coordinates);

  const auto& [a, b, c] = cast_coordinates.get_scaling();
  EXPECT_FLOAT_EQ(a, 1.0);
  EXPECT_FLOAT_EQ(b, 0.5);
  EXPECT_FLOAT_EQ(c, 2.0);
}


TEST_F(ParsingOfCameraParamAlone, GestBaseline) {
  auto box = parse_box(resources_path);
  const auto& camera_parameters =
      box->get_ref_to_contents().get_ref_to_camera_parameters();
  const auto& [x, y] = camera_parameters.get_baseline();
  EXPECT_FLOAT_EQ(x, SimpleCameraParameterContentsTest<float>::baseline_x);
  EXPECT_FLOAT_EQ(y, SimpleCameraParameterContentsTest<float>::baseline_y);
}


TEST_F(ParsingOfCameraParamAlone, MustThrowIfNotInitializedAndGettingAParam) {
  auto box = parse_box(resources_path);
  const auto& camera_parameters =
      box->get_ref_to_contents().get_ref_to_camera_parameters();
  EXPECT_THROW(camera_parameters.get<CameraParameterType::THETA_Y_CAM>({0, 0}),
      CameraParameterBoxExceptions::MissingCompleteInitializationException);
}


TEST_F(ParsingOfCameraParamAlone, ShouldNotThrowIfInitializedAndGettingAParam) {
  auto box = parse_box(resources_path);
  auto& camera_parameters =
      box->get_ref_to_contents().get_ref_to_camera_parameters();
  camera_parameters.initialize_missing_row_and_column(4,3);
  EXPECT_NO_THROW(camera_parameters.get<CameraParameterType::THETA_Y_CAM>({0, 0}));
}


TEST_F(ParsingOfCameraParamAlone, MustThrowIfInitializedWithInvalidValues) {
  auto box = parse_box(resources_path);
  auto& camera_parameters =
      box->get_ref_to_contents().get_ref_to_camera_parameters();
  EXPECT_THROW(camera_parameters.initialize_missing_row_and_column(4,4),CameraParameterBoxExceptions::InvalidLazzyInitializationException);
}



int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  // CameraParameterBox;

  return RUN_ALL_TESTS();
}