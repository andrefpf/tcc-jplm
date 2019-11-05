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

/** \file     LightfieldTests.cpp
 *  \brief    Test of lightfield.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#include <iostream>
#include "Lib/Part2/Common/Lightfield.h"
#include "Lib/Utils/Image/RGBImage.h"
#include "gtest/gtest.h"


TEST(InitialLightfieldTests, LightfieldHoldsGivenWidth) {
  Lightfield<uint16_t> lightfield(2, 3);
  EXPECT_EQ(lightfield.get_width(), 2);
}


TEST(InitialLightfieldTests, LightfieldHoldsGivenHeight) {
  Lightfield<uint16_t> lightfield(2, 3);
  EXPECT_EQ(lightfield.get_height(), 3);
}


struct LightfieldViewTests : public testing::Test {
 protected:
  Lightfield<uint16_t> lightfield;  // = Lightfield<uint16_t>(2,3);
  std::unique_ptr<RGBImage<uint16_t>> image =
      std::make_unique<RGBImage<uint16_t>>(1, 2, 10);
  View<uint16_t> view;
  LightfieldViewTests() : lightfield(2, 3), view(std::move(image)) {
    // view = View<uint16_t>(std::move(image));
  }
};


TEST_F(LightfieldViewTests, MoveViewKeepsLightfieldWidth) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(lightfield.get_width(), 2);
}


TEST_F(LightfieldViewTests, MoveViewKeepsLightfieldHeight) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(lightfield.get_height(), 3);
}


TEST_F(LightfieldViewTests, MoveViewKeepsViewWidth) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_width(), 1);
}


TEST_F(LightfieldViewTests, MoveViewKeepsViewHeight) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_height(), 2);
}


TEST_F(LightfieldViewTests, MoveViewKeepsViewBpp) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_bpp(), 10);
}


TEST_F(LightfieldViewTests, CopyViewKeepsLightfieldWidth) {
  lightfield.set_view_at(view, {0, 0});
  EXPECT_EQ(lightfield.get_width(), 2);
}


TEST_F(LightfieldViewTests, CopyViewKeepsLightfieldHeight) {
  lightfield.set_view_at(view, {0, 0});
  EXPECT_EQ(lightfield.get_height(), 3);
}


TEST_F(LightfieldViewTests, CopyViewKeepsWidth) {
  lightfield.set_view_at(view, {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_width(), 1);
}


TEST_F(LightfieldViewTests, CopyViewKeepsHeight) {
  lightfield.set_view_at(view, {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_height(), 2);
}


TEST_F(LightfieldViewTests, CopyViewKeepsViewBpp) {
  lightfield.set_view_at(view, {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_bpp(), 10);
}


TEST_F(LightfieldViewTests, LighfieldGetsViewsWidth) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(
      lightfield.get_view_at({0, 0}).get_width(), lightfield.get_views_width_u());
}


TEST_F(LightfieldViewTests, LighfieldGetsViewsHeight) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(lightfield.get_view_at({0, 0}).get_height(),
            lightfield.get_views_height_v());
}


TEST_F(LightfieldViewTests, LighfieldGetsViewsBpp) {
  lightfield.set_view_at(std::move(view), {0, 0});
  EXPECT_EQ(
      lightfield.get_view_at({0, 0}).get_bpp(), lightfield.get_views_bpp());
}


TEST_F(LightfieldViewTests, LighfieldGetsDimension) {
  lightfield.set_view_at(std::move(view), {0, 0});
  const auto [t, s, v, u] = lightfield.get_dimensions().get();
  EXPECT_EQ(t, 2);
  EXPECT_EQ(s, 3);
  EXPECT_EQ(v, 1);
  EXPECT_EQ(u, 2);
}


TEST_F(LightfieldViewTests, LighfieldGetsDimensionThroughStructuredBinding) {
  lightfield.set_view_at(std::move(view), {0, 0});
  const auto [t, s, v, u] = lightfield.get_dimensions();
  std::cout << "DEBUGA " << __FILE__ << ":" << __LINE__ << " t=" << t
      << " s=" << s
      << " u=" << u
      << " v=" << v << std::endl;
  EXPECT_EQ(t, 2);
  EXPECT_EQ(s, 3);
  EXPECT_EQ(v, 1);
  EXPECT_EQ(u, 2);
}


TEST_F(LightfieldViewTests, AViewCanBeAcessedByBracketOperator) {
  lightfield.set_view_at(std::move(view), {0, 0});
  auto my_view = lightfield[0][0];
  EXPECT_EQ(my_view.get_width(), lightfield.get_views_width_u());
  EXPECT_EQ(my_view.get_height(), lightfield.get_views_height_v());
  EXPECT_EQ(my_view.get_bpp(), lightfield.get_views_bpp());
}


TEST_F(LightfieldViewTests, AChannelCanBeAcessedByBracketOperator) {
  lightfield.set_view_at(std::move(view), {0, 0});
  auto my_channel = lightfield[0][0][0];
  EXPECT_EQ(my_channel.get_width(), lightfield.get_views_width_u());
  EXPECT_EQ(my_channel.get_height(), lightfield.get_views_height_v());
  EXPECT_EQ(my_channel.get_bpp(), lightfield.get_views_bpp());
}


TEST_F(LightfieldViewTests, APixelCanBeAcessedByBracketOperator) {
  lightfield.set_view_at(std::move(view), {0, 0});
  lightfield[0][0][0][0][0] = 25;
  lightfield[0][0][0][0][1] = 12;
  auto my_pixel = lightfield[0][0][0][0][0];
  auto my_pixel_2 = lightfield[0][0][0][0][1];
  EXPECT_EQ(my_pixel, 25);
  EXPECT_EQ(my_pixel_2, 12);
}


TEST(Anything, Test) {
  auto tst = std::make_unique<int[]>(0);
  if (!tst) {
    std::cout << "not alocated" << std::endl;
  } else {
    std::cout << "alocated" << std::endl;
    auto a = tst[0];
    std::cout << "alocated " << a << std::endl;
  }
}

// TEST_F(LightfieldViewTests, LighfieldGetsViewsBpp) {
//   lightfield.set_view_at(std::move(view), {0,0});
//   EXPECT_EQ(lightfield.get_view_at({0,0}).get_bpp(), lightfield.get_views_bpp());
// }

int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
