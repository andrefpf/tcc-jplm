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

/** \file     LightfieldCoordinateTests.cpp
 *  \brief    Test of lightfield.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-03
 */

#include <iostream>
#include "Lib/Part2/Common/LightfieldCoordinate.h"
#include "gtest/gtest.h"


struct SimpleLFCoordinateTest : public testing::Test {
 protected:
  std::size_t t = 2;
  std::size_t s = 4;
  std::size_t v = 6;
  std::size_t u = 8;
  std::unique_ptr<LightfieldCoordinate<std::size_t>> coordinate;

  SimpleLFCoordinateTest()
      : coordinate(
            std::make_unique<LightfieldCoordinate<std::size_t>>(t, s, v, u)){};
};


TEST_F(SimpleLFCoordinateTest, GetTMustGetInitialValues) {
  const auto [ret_t, ret_s, ret_v, ret_u] = coordinate->get();
  EXPECT_EQ(ret_t, t);
  EXPECT_EQ(ret_s, s);
  EXPECT_EQ(ret_v, v);
  EXPECT_EQ(ret_u, u);
}


TEST_F(
    SimpleLFCoordinateTest, CoordinateMustGetInitialValuesForStructuredBinding) {
  const auto [ret_t, ret_s, ret_v, ret_u] =
      *(coordinate.get());  //the current get is from the unique_ptr
  EXPECT_EQ(ret_t, t);
  EXPECT_EQ(ret_s, s);
  EXPECT_EQ(ret_v, v);
  EXPECT_EQ(ret_u, u);
}


TEST_F(SimpleLFCoordinateTest, GetTMustGetInitialT) {
  EXPECT_EQ(coordinate->get_t(), t);
}


TEST_F(SimpleLFCoordinateTest, GetSMustGetInitialS) {
  EXPECT_EQ(coordinate->get_s(), s);
}


TEST_F(SimpleLFCoordinateTest, GetVMustGetInitialV) {
  EXPECT_EQ(coordinate->get_v(), v);
}


TEST_F(SimpleLFCoordinateTest, GetUMustGetInitialU) {
  EXPECT_EQ(coordinate->get_u(), u);
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex0TMustGetInitialT) {
  EXPECT_EQ(coordinate->get<0>(), t);
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex1SMustGetInitialS) {
  EXPECT_EQ(coordinate->get<1>(), s);
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex2VMustGetInitialV) {
  EXPECT_EQ(coordinate->get<2>(), v);
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex3UMustGetInitialU) {
  EXPECT_EQ(coordinate->get<3>(), u);
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex0TMustGetSameAsGetT) {
  EXPECT_EQ(coordinate->get<0>(), coordinate->get_t());
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex1SMustGetSameAsGetS) {
  EXPECT_EQ(coordinate->get<1>(), coordinate->get_s());
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex2VMustGetSameAsGetV) {
  EXPECT_EQ(coordinate->get<2>(), coordinate->get_v());
}


TEST_F(SimpleLFCoordinateTest, GetUsingIndex3UMustGetSameAsGetU) {
  EXPECT_EQ(coordinate->get<3>(), coordinate->get_u());
}


TEST_F(SimpleLFCoordinateTest, SetMustChangeCoordinatesInTheNextGet) {
  coordinate->set({1, 2, 3, 4});
  const auto [ret_t, ret_s, ret_v, ret_u] = coordinate->get();
  EXPECT_EQ(ret_t, 1);
  EXPECT_EQ(ret_s, 2);
  EXPECT_EQ(ret_v, 3);
  EXPECT_EQ(ret_u, 4);
}


TEST_F(SimpleLFCoordinateTest, SetTMustChangeTInTheNextGet) {
  coordinate->set_t(coordinate->get_t() + 5);
  EXPECT_EQ(coordinate->get_t(), t + 5);
}


TEST_F(SimpleLFCoordinateTest, SetSMustChangeSInTheNextGet) {
  coordinate->set_s(coordinate->get_s() + 5);
  EXPECT_EQ(coordinate->get_s(), s + 5);
}


TEST_F(SimpleLFCoordinateTest, SetVMustChangeVInTheNextGet) {
  coordinate->set_v(coordinate->get_v() + 5);
  EXPECT_EQ(coordinate->get_v(), v + 5);
}


TEST_F(SimpleLFCoordinateTest, SetUMustChangeUInTheNextGet) {
  coordinate->set_u(coordinate->get_u() + 5);
  EXPECT_EQ(coordinate->get_u(), u + 5);
}


TEST_F(SimpleLFCoordinateTest, TwoDimentionsEquallyInitializedAreEqual) {
	auto other_Coordinate = LightfieldCoordinate<std::size_t>(t, s, v, u);
  EXPECT_EQ(*(coordinate.get()), other_Coordinate);
}


TEST_F(SimpleLFCoordinateTest, TwoDimentionsInitializedDifferentlyInTAreNotEqual) {
	auto other_Coordinate = LightfieldCoordinate<std::size_t>(t+1, s, v, u);
  EXPECT_NE(*(coordinate.get()), other_Coordinate);
}


TEST_F(SimpleLFCoordinateTest, TwoDimentionsInitializedDifferentlyInSAreNotEqual) {
	auto other_Coordinate = LightfieldCoordinate<std::size_t>(t, s+1, v, u);
  EXPECT_NE(*(coordinate.get()), other_Coordinate);
}


TEST_F(SimpleLFCoordinateTest, TwoDimentionsInitializedDifferentlyInVAreNotEqual) {
	auto other_Coordinate = LightfieldCoordinate<std::size_t>(t, s, v+1, u);
  EXPECT_NE(*(coordinate.get()), other_Coordinate);
}


TEST_F(SimpleLFCoordinateTest, TwoDimentionsInitializedDifferentlyInUAreNotEqual) {
	auto other_Coordinate = LightfieldCoordinate<std::size_t>(t, s, v, u+1);
  EXPECT_NE(*(coordinate.get()), other_Coordinate);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
