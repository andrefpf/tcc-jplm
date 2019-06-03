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

/** \file     ViewTests.cpp
 *  \brief    Test of lightfield view.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#include <iostream>
#include "Lib/Part2/Common/View.h"
#include "gtest/gtest.h"


TEST(InitialViewTests, ViewHasSameWidthAsItsInitializerImage) {
  auto image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
  auto image_width = image->get_width();
  auto view = View<uint16_t>(std::move(image));
  EXPECT_EQ(view.get_width(), image_width);
}


TEST(InitialViewTests, ViewHasSameHeightAsItsInitializerImage) {
  auto image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
  auto image_height = image->get_height();
  auto view = View<uint16_t>(std::move(image));
  EXPECT_EQ(view.get_height(), image_height);
}


TEST(InitialViewTests, ViewHasSameBppAsItsInitializerImage) {
  auto image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
  auto image_bpp = image->get_bpp();
  auto view = View<uint16_t>(std::move(image));
  EXPECT_EQ(view.get_bpp(), image_bpp);
}


TEST(InitialViewTests, ViewHasSameTypeAsItsInitializerImage) {
  auto image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
  auto image_type = image->get_type();
  auto view = View<uint16_t>(std::move(image));
  EXPECT_EQ(view.get_image_type(), image_type);
}


struct ImageInViewTests : public testing::Test {
protected:
  View<uint16_t> view;
  ImageInViewTests(){
    std::unique_ptr<Image<uint16_t>> image = std::make_unique<RGBImage<uint16_t>>(1,2,10);
    auto as_three_channel = static_cast<ThreeChannelImage<uint16_t>*>(image.get());
    as_three_channel->set_pixel_at({1023,1023,1023}, 0,0);
    as_three_channel->set_pixel_at({1023,0,0}, 0,1);
    view = std::move(image);
  }
};


TEST_F(ImageInViewTests, ViewAssigmentFromImageWidth) {
  EXPECT_EQ(view.get_width(), 1);
}


TEST_F(ImageInViewTests, ViewAssigmentFromImageHeight) {
  EXPECT_EQ(view.get_height(), 2);
}


TEST_F(ImageInViewTests, ViewAssigmentFromImageBpp) {
  EXPECT_EQ(view.get_bpp(), 10);
}


TEST_F(ImageInViewTests, CanGetWhitePixelFromView) {
  const auto& [r, g, b] = view.get_pixel_at({0,0});
  EXPECT_EQ(r, 1023);
  EXPECT_EQ(g, 1023);
  EXPECT_EQ(b, 1023);
}


TEST_F(ImageInViewTests, CanGetRedPixelFromView) {
  const auto& [r, g, b] = view.get_pixel_at({0,1});
  EXPECT_EQ(r, 1023);
  EXPECT_EQ(g, 0);
  EXPECT_EQ(b, 0);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
