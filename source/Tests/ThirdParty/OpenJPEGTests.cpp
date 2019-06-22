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

/** \file     OpenJPEGTests.cpp
 *  \brief    Tests for the external library OpenJPEG
 *  \details  See more details about it at
 *            https://github.com/uclouvain/openjpeg
 *            https://www.openjpeg.org/
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-06-18
 */

#include "gtest/gtest.h"
#include "openjpeg.h"

TEST(OpenJP2K, Openjp2_Openjpeg_h) {
  EXPECT_EQ(OPJ_PROFILE_NONE, 0x0000);
  EXPECT_EQ(OPJ_PROFILE_0, 0x0001);
  EXPECT_EQ(OPJ_PROFILE_1, 0x0002);
  EXPECT_EQ(OPJ_PROFILE_PART2, 0x8000);
  EXPECT_EQ(OPJ_PROFILE_CINEMA_2K, 0x0003);
  EXPECT_EQ(OPJ_PROFILE_CINEMA_4K, 0x0004);
  EXPECT_EQ(OPJ_PROFILE_CINEMA_S2K, 0x0005);
  EXPECT_EQ(OPJ_PROFILE_CINEMA_S4K, 0x0006);
  EXPECT_EQ(OPJ_PROFILE_CINEMA_LTS, 0x0007);
  EXPECT_EQ(OPJ_PROFILE_BC_SINGLE, 0x0100);
  EXPECT_EQ(OPJ_PROFILE_BC_MULTI, 0x0200);
  EXPECT_EQ(OPJ_PROFILE_BC_MULTI_R, 0x0300);
  EXPECT_EQ(OPJ_PROFILE_IMF_2K, 0x0400);
  EXPECT_EQ(OPJ_PROFILE_IMF_4K, 0x0401);
  EXPECT_EQ(OPJ_PROFILE_IMF_8K, 0x0402);
  EXPECT_EQ(OPJ_PROFILE_IMF_2K_R, 0x0403);
  EXPECT_EQ(OPJ_PROFILE_IMF_4K_R, 0x0800);
  EXPECT_EQ(OPJ_PROFILE_IMF_8K_R, 0x0801);
  EXPECT_EQ(OPJ_EXTENSION_NONE, 0x0000);
  EXPECT_EQ(OPJ_EXTENSION_MCT, 0x0100);
  EXPECT_EQ(OPJ_CINEMA_24_CS, 1302083);
  EXPECT_EQ(OPJ_CINEMA_48_CS, 651041);
  EXPECT_EQ(OPJ_CINEMA_24_COMP, 1041666);
  EXPECT_EQ(OPJ_CINEMA_48_COMP, 520833);
  EXPECT_EQ(OPJ_DPARAMETERS_IGNORE_PCLR_CMAP_CDEF_FLAG, 0x0001);
  EXPECT_EQ(OPJ_DPARAMETERS_DUMP_FLAG, 0x0002);
}



int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
