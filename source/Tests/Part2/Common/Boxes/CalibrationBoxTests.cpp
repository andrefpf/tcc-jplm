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

/** \file     CalibrationBoxTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */


#include <iostream>
#include "Lib/Part2/Common/Boxes/CalibrationBox.h"
#include "gtest/gtest.h"

TEST(BasicTests, HasCorrectId) {
	EXPECT_EQ(CalibrationBox::id, 0x6c666361);
}


TEST(FloatingPointCoordinatesTests, FloatPPis1) {
	auto coordinates = FloatingPointCoordinates<float>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.get_pp(), 1);
}


TEST(FloatingPointCoordinatesTests, FloatSizeIs9Times4) {
	auto coordinates = FloatingPointCoordinates<float>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.size(), 36);
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


TEST(FloatingPointCoordinatesTests, DoubleSizeIs9Times8) {
	auto coordinates = FloatingPointCoordinates<double>({0,0,0}, {0,0,0}, {1,1,1});
	EXPECT_EQ(coordinates.size(), 72);
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
	EXPECT_EQ(camera_parameter_array.size(), ((13*13)+11+2)*sizeof(float));
}


TEST(CameraParametersArrayTests, InitializationWithExtInt2) {
	auto camera_parameter_array = CameraParametersArray({0.0,0.0}, 13, 13, 0x2);
	EXPECT_EQ(camera_parameter_array.size(), ((13*13)+11+2)*sizeof(float));
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
	EXPECT_EQ(camera_parameter_array.size(), (((13*13)*(ones))+(12-ones)+2)*sizeof(float));
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
  EXPECT_EQ(camera_parameter_array.size(), (((13*13)*(ones))+(12-ones)+2)*sizeof(float));
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

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}