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
#include "gtest/gtest.h"


// TEST(InitialLightfieldTests, LightfieldHoldsGivenWidth) {
// 	Lightfield<uint16_t> lightfield(2,3);
// 	EXPECT_EQ(lightfield.get_width(), 2);
// }


// TEST(InitialLightfieldTests, LightfieldHoldsGivenHeight) {
// 	Lightfield<uint16_t> lightfield(2,3);
// 	EXPECT_EQ(lightfield.get_height(), 3);
// }

TEST(InitialLightfieldTests, MoveView) {
  auto image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
  auto view = View<uint16_t>(std::move(image));
  auto lightfield = Lightfield<uint16_t>(2,3);
  std::cout << "Created lightfield" << std::endl;
  lightfield.set_view_at(std::move(view), {0,0});
  std::cout << "set view at lightfield" << std::endl;
  EXPECT_EQ(lightfield.get_view_at({0,0}).get_width(), 1);
  EXPECT_EQ(lightfield.get_view_at({0,0}).get_height(), 2);
}


TEST(InitialLightfieldTests, CopyView) {
  auto image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
  auto view = View<uint16_t>(std::move(image));
  auto lightfield = Lightfield<uint16_t>(2,3);
  std::cout << "Created lightfield" << std::endl;
  lightfield.set_view_at(view, {0,0});
  std::cout << "set view at lightfield" << std::endl;
  EXPECT_EQ(lightfield.get_view_at({0,0}).get_width(), 1);
  EXPECT_EQ(lightfield.get_view_at({0,0}).get_height(), 2);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
