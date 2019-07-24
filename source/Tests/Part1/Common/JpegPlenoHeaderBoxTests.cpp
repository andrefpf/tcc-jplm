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

/** \file     JpegPlenoHeaderBoxTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-24
 */


#include <iostream>
#include "Lib/Part1/Common/JpegPlenoHeaderBox.h"
#include "gtest/gtest.h"


TEST(JpegPlenoHeaderContentsBasic, Initialization) {
  EXPECT_NO_THROW(JpegPlenoHeaderContents());
}


TEST(JpegPlenoHeaderBoxBasic, Initialization) {
  EXPECT_NO_THROW(auto jpeg_pleno_header_box = JpegPlenoHeaderBox(JpegPlenoHeaderContents()));
}


TEST(JpegPlenoHeaderBoxBasic, CorrectTBoxValue) {
  auto jpeg_pleno_header_box = JpegPlenoHeaderBox(JpegPlenoHeaderContents());
  EXPECT_EQ(jpeg_pleno_header_box.get_tbox().get_value(), 0x6a706c68);
}


TEST(JpegPlenoHeaderBoxBasic, CorrectLBoxValue) {
  auto jpeg_pleno_header_box = JpegPlenoHeaderBox(JpegPlenoHeaderContents());
  EXPECT_EQ(jpeg_pleno_header_box.get_lbox().get_value(), 20);
  //20: 4 from LBox, 4 from Tbox, 8 from N_LF, 8 from N_PC, 8 from N_HO
}


TEST(JpegPlenoHeaderBoxBasic, CorrectLBoxValueWhenAddedOneLF) {
  auto header_contents = JpegPlenoHeaderContents();
  header_contents.add_pointer_to_light_field_box(0);
  auto jpeg_pleno_header_box = JpegPlenoHeaderBox(header_contents);
  EXPECT_EQ(jpeg_pleno_header_box.get_lbox().get_value(), 28);
}


TEST(JpegPlenoHeaderBoxBasic, CorrectLBoxValueWhenAddedOnePC) {
  auto header_contents = JpegPlenoHeaderContents();
  header_contents.add_pointer_to_point_cloud_box(0);
  auto jpeg_pleno_header_box = JpegPlenoHeaderBox(header_contents);
  EXPECT_EQ(jpeg_pleno_header_box.get_lbox().get_value(), 28);
}


TEST(JpegPlenoHeaderBoxBasic, CorrectLBoxValueWhenAddedOneHo) {
  auto header_contents = JpegPlenoHeaderContents();
  header_contents.add_pointer_to_hologram_box(0);
  auto jpeg_pleno_header_box = JpegPlenoHeaderBox(header_contents);
  EXPECT_EQ(jpeg_pleno_header_box.get_lbox().get_value(), 28);
}


TEST(JpegPlenoHeaderBoxBasic, CorrectLBoxValueWhenAddedOneOfEachType) {
  auto header_contents = JpegPlenoHeaderContents();
  header_contents.add_pointer_to_light_field_box(0);
  header_contents.add_pointer_to_point_cloud_box(0);
  header_contents.add_pointer_to_hologram_box(0);
  auto jpeg_pleno_header_box = JpegPlenoHeaderBox(header_contents);
  EXPECT_EQ(jpeg_pleno_header_box.get_lbox().get_value(), 20+24);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}