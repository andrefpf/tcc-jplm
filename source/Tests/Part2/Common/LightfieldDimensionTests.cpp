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

/** \file     LightfieldDimensionTests.cpp
 *  \brief    Test of lightfield.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-03
 */

#include <iostream>
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "gtest/gtest.h"

struct SimpleLFDimensionTest : public testing::Test {
 protected:
  std::size_t t = 2;
  std::size_t s = 4;
  std::size_t v = 6;
  std::size_t u = 8;
  std::unique_ptr<LightfieldDimension<std::size_t>> dimension;

  SimpleLFDimensionTest()
      : dimension(
            std::make_unique<LightfieldDimension<std::size_t>>(t, s, v, u)){};
};


TEST_F(SimpleLFDimensionTest, GetTMustGetInitialValues) {
  const auto [ret_t, ret_s, ret_v, ret_u] = dimension->get();
  EXPECT_EQ(ret_t, t);
  EXPECT_EQ(ret_s, s);
  EXPECT_EQ(ret_v, v);
  EXPECT_EQ(ret_u, u);
}

TEST_F(
    SimpleLFDimensionTest, DimensionMustGetInitialValuesForStructuredBinding) {
  const auto [ret_t, ret_s, ret_v, ret_u] =
      *(dimension.get());  //the current get is from the unique_ptr
  EXPECT_EQ(ret_t, t);
  EXPECT_EQ(ret_s, s);
  EXPECT_EQ(ret_v, v);
  EXPECT_EQ(ret_u, u);
}

TEST_F(SimpleLFDimensionTest, GetTMustGetInitialT) {
  EXPECT_EQ(dimension->get_t(), t);
}


TEST_F(SimpleLFDimensionTest, GetSMustGetInitialS) {
  EXPECT_EQ(dimension->get_s(), s);
}


TEST_F(SimpleLFDimensionTest, GetVMustGetInitialV) {
  EXPECT_EQ(dimension->get_v(), v);
}


TEST_F(SimpleLFDimensionTest, GetUMustGetInitialU) {
  EXPECT_EQ(dimension->get_u(), u);
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex0TMustGetInitialT) {
  EXPECT_EQ(dimension->get<0>(), t);
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex1SMustGetInitialS) {
  EXPECT_EQ(dimension->get<1>(), s);
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex2VMustGetInitialV) {
  EXPECT_EQ(dimension->get<2>(), v);
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex3UMustGetInitialU) {
  EXPECT_EQ(dimension->get<3>(), u);
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex0TMustGetSameAsGetT) {
  EXPECT_EQ(dimension->get<0>(), dimension->get_t());
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex1SMustGetSameAsGetS) {
  EXPECT_EQ(dimension->get<1>(), dimension->get_s());
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex2VMustGetSameAsGetV) {
  EXPECT_EQ(dimension->get<2>(), dimension->get_v());
}


TEST_F(SimpleLFDimensionTest, GetUsingIndex3UMustGetSameAsGetU) {
  EXPECT_EQ(dimension->get<3>(), dimension->get_u());
}


TEST_F(SimpleLFDimensionTest, SetMustChangeDimensionsInTheNextGet) {
  dimension->set({1, 2, 3, 4});
  const auto [ret_t, ret_s, ret_v, ret_u] = dimension->get();
  EXPECT_EQ(ret_t, 1);
  EXPECT_EQ(ret_s, 2);
  EXPECT_EQ(ret_v, 3);
  EXPECT_EQ(ret_u, 4);
}


TEST_F(SimpleLFDimensionTest, SetTMustChangeTInTheNextGet) {
  dimension->set_t(dimension->get_t() + 5);
  EXPECT_EQ(dimension->get_t(), t + 5);
}


TEST_F(SimpleLFDimensionTest, SetSMustChangeSInTheNextGet) {
  dimension->set_s(dimension->get_s() + 5);
  EXPECT_EQ(dimension->get_s(), s + 5);
}


TEST_F(SimpleLFDimensionTest, SetVMustChangeVInTheNextGet) {
  dimension->set_v(dimension->get_v() + 5);
  EXPECT_EQ(dimension->get_v(), v + 5);
}


TEST_F(SimpleLFDimensionTest, SetUMustChangeUInTheNextGet) {
  dimension->set_u(dimension->get_u() + 5);
  EXPECT_EQ(dimension->get_u(), u + 5);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
