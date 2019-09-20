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

/** \file     CameraParameterBoxTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */


#include <iostream>
#include "Lib/Part2/Common/Boxes/CameraParameterBox.h"
#include "gtest/gtest.h"


TEST(BasicTests, HasCorrectId) {
	EXPECT_EQ(CameraParameterBox::id, 0x6c666370);
}


TEST(FloatingPointCoordinatesTests, FloatPPis1) {
	auto coordinates = FloatingPointCoordinates<float>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.get_pp(), 1);
}


TEST(FloatingPointCoordinatesTests, FloatSizeIs9Times4Plus1) {
	auto coordinates = FloatingPointCoordinates<float>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.size(), 37);
}


TEST(FloatingPointCoordinatesTests, FloatGetOriginPosition) {
	auto coordinates = FloatingPointCoordinates<float>({1,2,3}, {4,5,6}, {1,1,1});
	const auto& [x, y, z] = coordinates.get_origin_position();
	EXPECT_EQ(x, 1.0);
	EXPECT_EQ(y, 2.0);
	EXPECT_EQ(z, 3.0);
}


TEST(FloatingPointCoordinatesTests, FloatGetRotationAroundAxis) {
	auto coordinates = FloatingPointCoordinates<float>({1,2,3}, {4,5,6}, {1,1,1});
	const auto& [x, y, z] = coordinates.get_rotation_around_axis();
	EXPECT_EQ(x, 4.0);
	EXPECT_EQ(y, 5.0);
	EXPECT_EQ(z, 6.0);
}


TEST(FloatingPointCoordinatesTests, FloatGetScaling) {
	auto coordinates = FloatingPointCoordinates<float>({1,2,3}, {4,5,6}, {1,2,4});
	const auto& [x, y, z] = coordinates.get_scaling();
	EXPECT_EQ(x, 1.0);
	EXPECT_EQ(y, 2.0);
	EXPECT_EQ(z, 4.0);
}


TEST(FloatingPointCoordinatesTests, DoublePPis2) {
	auto coordinates = FloatingPointCoordinates<double>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.get_pp(), 2);
}


TEST(FloatingPointCoordinatesTests, DoubleSizeIs9Times8Plus1) {
	auto coordinates = FloatingPointCoordinates<double>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.size(), 73);
}


TEST(FloatingPointCoordinatesTests, DoubleGetOriginPosition) {
	auto coordinates = FloatingPointCoordinates<double>({1,2,3}, {4,5,6}, {1,1,1});
	const auto& [x, y, z] = coordinates.get_origin_position();
	EXPECT_EQ(x, 1.0);
	EXPECT_EQ(y, 2.0);
	EXPECT_EQ(z, 3.0);
}


TEST(FloatingPointCoordinatesTests, DoubleGetRotationAroundAxis) {
	auto coordinates = FloatingPointCoordinates<double>({1,2,3}, {4,5,6}, {1,1,1});
	const auto& [x, y, z] = coordinates.get_rotation_around_axis();
	EXPECT_EQ(x, 4.0);
	EXPECT_EQ(y, 5.0);
	EXPECT_EQ(z, 6.0);
}


TEST(FloatingPointCoordinatesTests, DoubleGetScaling) {
	auto coordinates = FloatingPointCoordinates<double>({1,2,3}, {4,5,6}, {1,2,4});
	const auto& [x, y, z] = coordinates.get_scaling();
	EXPECT_EQ(x, 1.0);
	EXPECT_EQ(y, 2.0);
	EXPECT_EQ(z, 4.0);
}


TEST(FloatingPointCoordinatesTests, FloatGetCoordinatesFromBytes) {
	auto coordinates = FloatingPointCoordinates<float>({12,2,3}, {4,5,6}, {1,2,4});
	const auto& [x, y, z] = coordinates.get_origin_position();
	auto bytes = coordinates.get_bytes();
	// the 1 is necessary because the first byte represents pp;
	const auto& [a, b, c] = BinaryTools::get_tuple_from_big_endian_byte_vector<float, float, float>(bytes, 1);
	EXPECT_EQ(x, a);
	EXPECT_EQ(y, b);
	EXPECT_EQ(z, c);

	// auto count = 0;
	// for(const auto& byte: bytes) {
	// 	std::cout << std::to_integer<int>(byte) << " ";
	// 	++count;
	// 	if(count == 1)
	// 		count+=3;
	// 	if(count%4 == 0)
	// 		std::cout << std::endl;
	// }
}


TEST(CameraParametersArrayTests, InitializationWithExtInt) {
	auto camera_parameter_array = CameraParametersArray({0.0,0.0}, 13, 13, 0x1);
	EXPECT_EQ(camera_parameter_array.size(), ((13*13)+11+2)*sizeof(float)+2);
}


TEST(CameraParametersArrayTests, InitializationWithExtInt2) {
	auto camera_parameter_array = CameraParametersArray({0.0,0.0}, 13, 13, 0x2);
	EXPECT_EQ(camera_parameter_array.size(), ((13*13)+11+2)*sizeof(float)+2);
}


std::size_t count_ones(uint16_t value) {
	std::size_t ones = 0;
	for(auto i = 0; i<16; ++i) {
		if(value&0x1){
			ones++;
		}
		value>>=1;
	}
	return ones;
}


TEST(CameraParametersArrayTests, InitializationWithExtOnes) {
	uint16_t ext_int = 0x4d;
	auto camera_parameter_array = CameraParametersArray({0.0,0.0}, 13, 13, ext_int);
	auto ones = count_ones(ext_int);
	EXPECT_EQ(ones, 4);
	EXPECT_EQ(camera_parameter_array.size(), (((13*13)*(ones))+(12-ones)+2)*sizeof(float)+2);
}


class ExtIntTest : public ::testing::TestWithParam<int> {
 public:
  ExtIntTest() = default;
  ~ExtIntTest() = default;
};


INSTANTIATE_TEST_SUITE_P(AUTO_GEN_OH, ExtIntTest,
    ::testing::Range(
        0, 4095, 1));


TEST_P(ExtIntTest, GetExtIntIsEqualItsInitialization) {
  uint16_t ext_int = static_cast<uint16_t>(GetParam());
  auto camera_parameter_array = CameraParametersArray({0.0,0.0}, 13, 13, ext_int);
  EXPECT_EQ(ext_int, camera_parameter_array.get_ext_int_bits());
}


TEST_P(ExtIntTest, GetCorrectSize) {
  uint16_t ext_int = static_cast<uint16_t>(GetParam());
  auto ones = count_ones(ext_int);
  auto camera_parameter_array = CameraParametersArray({0.0,0.0}, 13, 13, ext_int);
  EXPECT_EQ(camera_parameter_array.size(), (((13*13)*(ones))+(12-ones)+2)*sizeof(float)+2);
}


TEST(CameraParametersArrayBaselineTests, CorrectBaselineX) {
	auto camera_parameter_array = CameraParametersArray({1.0,2.0}, 13, 13, 0x1);
	EXPECT_EQ(camera_parameter_array.get_baseline_x(), 1.0);
}


TEST(CameraParametersArrayBaselineTests, CorrectBaselineY) {
	auto camera_parameter_array = CameraParametersArray({1.0,2.0}, 13, 13, 0x1);
	EXPECT_EQ(camera_parameter_array.get_baseline_y(), 2.0);
}


TEST(CameraParametersArrayBaselineTests, CorrectBaselineTuple) {
	auto baseline = std::tuple<float, float>(1.0,2.0);
	auto camera_parameter_array = CameraParametersArray(baseline, 13, 13, 0x1);
	EXPECT_EQ(camera_parameter_array.get_baseline(), baseline);
}


TEST(CameraParametersArrayBaselineTests, GetsTheYCCUsingBaselineAndTValue) {
	auto baseline = std::tuple<float, float>(1.0,2.0);
	auto camera_parameter_array = CameraParametersArray(baseline, 13, 13, 0x1);
	EXPECT_EQ(camera_parameter_array.get<YCC>({11, 10}), 22.0);
}


template<typename T>
struct SimpleCameraParameterContentsTest : public testing::TestWithParam<T> {
 protected:
  std::tuple<T, T, T> xyz_coordinates;
  std::tuple<T, T, T> xyz_angles;
  std::tuple<T, T, T> xyz_scalings;
  float baseline_x = 3.1415;  //dummy, just to check if io works
  float baseline_y = 55.625;
  float y00 = 115.0;
  float z00 = 42.0;
  float theta_x = 1.25;
  float sk = 1;
  float u0 = 0.111;
  uint32_t rows = 3;  //3x4 lightfield
  uint32_t columns = 4;  //3x4 lightfield
  uint16_t ExtInt = 0x9F1;  //is 100111110001
  float initial_value = 0.789101112;
  const float const_add_value = 0.042;
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

    for (auto& value : xcc_vec) {
      value = initial_value;
      initial_value += const_add_value;
    }
    for (auto& value : theta_y_vec) {
      value = initial_value;
      initial_value += const_add_value;
    }
    for (auto& value : theta_z_vec) {
      value = initial_value;
      initial_value += const_add_value;
    }
    for (auto& value : f_vec) {
      value = initial_value;
      initial_value += const_add_value;
    }
    for (auto& value : sW_vec) {
      value = initial_value;
      initial_value += const_add_value;
    }
    for (auto& value : sH_vec) {
      value = initial_value;
      initial_value += const_add_value;
    }
    for (auto& value : v0_vec) {
      value = initial_value;
      initial_value += const_add_value;
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


struct SimpleCameraParameterContentsTestFloat
    : public SimpleCameraParameterContentsTest<float> {};


TEST_F(SimpleCameraParameterContentsTestFloat, GetsFPCoordinatesSizeRight) {
  auto camera_parameter_contents = get_contents();
  EXPECT_EQ(camera_parameter_contents->get_ref_to_coordinates().size(),
      expected_fp_coordinates_size());
}


TEST_F(SimpleCameraParameterContentsTestFloat, GetsCameraParametersSizeRight) {
  auto camera_parameter_contents = get_contents();
  EXPECT_EQ(camera_parameter_contents->get_ref_to_camera_parameters().size(),
      expected_camera_parameter_size());
}


TEST_F(SimpleCameraParameterContentsTestFloat, GetsSizeRight) {
  auto camera_parameter_contents = get_contents();
  EXPECT_EQ(camera_parameter_contents->size(), expected_size());
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetsByteVectorOfFPCoordinatesSizeRight) {
  auto camera_parameter_contents = get_contents();
  EXPECT_EQ(
      camera_parameter_contents->get_ref_to_coordinates().get_bytes().size(),
      expected_fp_coordinates_size());
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetsByteVectorOfCameraParametersSizeRight) {
  auto camera_parameter_contents = get_contents();
  EXPECT_EQ(camera_parameter_contents->get_ref_to_camera_parameters()
                .get_bytes()
                .size(),
      expected_camera_parameter_size());
}


TEST_F(SimpleCameraParameterContentsTestFloat, GetsByteVectorOfSizeRight) {
  auto camera_parameter_contents = get_contents();
  EXPECT_EQ(camera_parameter_contents->get_bytes().size(), expected_size());
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetsRefToCoordinatesOriginPosition) {
  auto camera_parameter_contents = get_contents();
  const auto& coordinates = camera_parameter_contents->get_ref_to_coordinates();
  //need to cast :(
  const auto& cast_coordinates =
      static_cast<const FloatingPointCoordinates<float>&>(coordinates);
  EXPECT_EQ(cast_coordinates.get_origin_position(), xyz_coordinates);
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetsRefToCoordinatesRotationArondAxis) {
  auto camera_parameter_contents = get_contents();
  const auto& coordinates = camera_parameter_contents->get_ref_to_coordinates();
  //need to cast :(
  const auto& cast_coordinates =
      static_cast<const FloatingPointCoordinates<float>&>(coordinates);
  EXPECT_EQ(cast_coordinates.get_rotation_around_axis(), xyz_angles);
}


TEST_F(SimpleCameraParameterContentsTestFloat, GetsRefToCoordinatesScaling) {
  auto camera_parameter_contents = get_contents();
  const auto& coordinates = camera_parameter_contents->get_ref_to_coordinates();
  //need to cast :(
  const auto& cast_coordinates =
      static_cast<const FloatingPointCoordinates<float>&>(coordinates);
  EXPECT_EQ(cast_coordinates.get_scaling(), xyz_scalings);
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetValuesFromXCCVecInCameraContents) {
  auto camera_parameter_contents = get_contents();
  auto count = 0;
  for (auto i = decltype(rows){0}; i < rows; ++i) {
    for (auto j = decltype(columns){0}; j < columns; ++j) {
      EXPECT_EQ(camera_parameter_contents->get<CameraParameterType::XCC>({i, j}), xcc_vec[count++]);
    }
  }
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetValuesFromXCCVecInCameraParameters) {
  auto camera_parameter_contents = get_contents();
  const auto& params =
      camera_parameter_contents->get_ref_to_camera_parameters();
  auto count = 0;
  for (auto i = decltype(rows){0}; i < rows; ++i) {
    for (auto j = decltype(columns){0}; j < columns; ++j) {
      EXPECT_EQ(params.get<CameraParameterType::XCC>({i, j}), xcc_vec[count++]);
    }
  }
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetValuesFromThetaYVecInCameraContents) {
  auto camera_parameter_contents = get_contents();
  auto count = 0;
  for (auto i = decltype(rows){0}; i < rows; ++i) {
    for (auto j = decltype(columns){0}; j < columns; ++j) {
      EXPECT_EQ(camera_parameter_contents->get<CameraParameterType::THETA_Y_CAM>({i, j}), theta_y_vec[count++]);
    }
  }
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetValuesFromThetaYVecInCameraParameters) {
  auto camera_parameter_contents = get_contents();
  const auto& params =
      camera_parameter_contents->get_ref_to_camera_parameters();
  auto count = 0;
  for (auto i = decltype(rows){0}; i < rows; ++i) {
    for (auto j = decltype(columns){0}; j < columns; ++j) {
      EXPECT_EQ(params.get<CameraParameterType::THETA_Y_CAM>({i, j}), theta_y_vec[count++]);
    }
  }
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetYCCValuesInCameraContents) {
  auto camera_parameter_contents = get_contents();
  const auto& params =
      camera_parameter_contents->get_ref_to_camera_parameters();
  auto baseline_y = params.get_baseline_y();
  for (auto i = decltype(rows){0}; i < rows; ++i) {
    for (auto j = decltype(columns){0}; j < columns; ++j) {
      EXPECT_EQ(camera_parameter_contents->get<CameraParameterType::YCC>({i, j}), i*baseline_y+y00);
    }
  }
}


TEST_F(SimpleCameraParameterContentsTestFloat,
    GetYCCValuesInCameraParameters) {
  auto camera_parameter_contents = get_contents();
  const auto& params =
      camera_parameter_contents->get_ref_to_camera_parameters();
  auto baseline_y = params.get_baseline_y();
  for (auto i = decltype(rows){0}; i < rows; ++i) {
    for (auto j = decltype(columns){0}; j < columns; ++j) {
      EXPECT_EQ(params.get<CameraParameterType::YCC>({i, j}), i*baseline_y+y00);
    }
  }
}

//! \todo if possible it would be nice to implement more tests. Also possible to use a double

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
