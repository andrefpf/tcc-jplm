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

/** \file     LightFieldHeaderContentsTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-26
 */


#include <iostream>
#include "Lib/Part2/Common/Boxes/LightFieldHeaderContents.h"
#include "gtest/gtest.h"


TEST(BasicTests, NoZeroT) {
  EXPECT_THROW(auto lf_header_contents = LightFieldHeaderContents({0, 1, 2, 3},
                   3, 8, CompressionTypeLightField::transform_mode),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(BasicTests, NoZeroS) {
  EXPECT_THROW(auto lf_header_contents = LightFieldHeaderContents({1, 0, 2, 3},
                   3, 8, CompressionTypeLightField::transform_mode),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(BasicTests, NoZeroV) {
  EXPECT_THROW(auto lf_header_contents = LightFieldHeaderContents({1, 2, 0, 3},
                   3, 8, CompressionTypeLightField::transform_mode),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(BasicTests, NoZeroU) {
  EXPECT_THROW(auto lf_header_contents = LightFieldHeaderContents({1, 2, 3, 0},
                   3, 8, CompressionTypeLightField::transform_mode),
      LightfieldDimensionExceptions::InvalidZeroDimensionException);
}


TEST(BasicTests, gets_T) {
	auto lf_header_contents = LightFieldHeaderContents({42, 1, 2, 3},
		3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.get_light_field_dimension().get_t(), 42);
}


TEST(BasicTests, gets_S) {
	auto lf_header_contents = LightFieldHeaderContents({1, 42, 2, 3},
		3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.get_light_field_dimension().get_s(), 42);
}


TEST(BasicTests, gets_V) {
	auto lf_header_contents = LightFieldHeaderContents({1, 2, 42, 3},
		3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.get_light_field_dimension().get_v(), 42);
}


TEST(BasicTests, gets_U) {
	auto lf_header_contents = LightFieldHeaderContents({1, 2, 3, 42},
		3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.get_light_field_dimension().get_u(), 42);
}


TEST(BasicTests, gets_bpc) {
	auto lf_header_contents = LightFieldHeaderContents({1, 2, 3, 42},
		3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.get_bits_per_component(), 8);
}


TEST(BasicTests, gets_nc) {
	auto lf_header_contents = LightFieldHeaderContents({1, 2, 3, 42},
		3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.get_number_of_components(), 3);
}


TEST(BasicTests, Has39Bytes) {
  auto lf_header_contents = LightFieldHeaderContents({1, 2, 3, 42},
    3, 8, CompressionTypeLightField::transform_mode);
  EXPECT_EQ(lf_header_contents.size(), 22);
}

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}