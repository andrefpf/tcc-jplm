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

/** \file     ConfigurationParserTests.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2019-09-27
 */

#include <exception>
#include "Lib/Common/JPLMConfiguration.h"
#include "Lib/Common/JPLMConfigurationExceptions.h"
#include "Lib/Common/JPLMEncoderConfiguration.h"
#include "gtest/gtest.h"

using namespace std;


TEST(JPLMConfiguration, SimpleTest) {
  const char* argv[] = {"", "-i", "../cfg/part2/mule/I01Bikes.cfg"};
  int argc = 3;
  JPLMConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("../cfg/part2/mule/I01Bikes.cfg", config.getInput().c_str());
}

TEST(JPLMConfiguration, SimpleTestWithNonExpectedParameter) {
  const char* argv[] = {"", "-i", "../cfg/part2/mule/I01Bikes.cfg", "--alface"};
  int argc = 4;
  EXPECT_THROW({ JPLMConfiguration config(argc, const_cast<char**>(argv)); },
      std::runtime_error);
}


TEST(JPLMEncoderConfiguration, SimpleCLITest) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-c",
      "../cfg/part2/4DTransformMode/I01_Bikes_22016.json", "-p", "2", "-t",
      "13", "-s", "13", "-v", "434", "-u", "626"};
  int argc = 15;
  JPLMEncoderConfiguration config(argc, const_cast<char**>(argv));
  EXPECT_STREQ("../resources/small_greek/", config.getInput().c_str());
  EXPECT_EQ(JpegPlenoPart::LightField, config.get_jpeg_pleno_part());
  EXPECT_EQ(13, config.getNumberOfRowsT());
  EXPECT_EQ(13, config.getNumberOfColumnsS());
  EXPECT_EQ(434, config.getViewHeightV());
  EXPECT_EQ(626, config.getViewWidthU());
}


TEST(JPLMEncoderConfiguration, RaiseErrorWhetherConfigNotExists) {
  const char* argv[] = {"", "-i", "../resources/small_greek/", "-c",
      "/tmp/donotcreateme/I01Bikes.cfg"};
  int argc = 5;

  EXPECT_THROW(
      { JPLMEncoderConfiguration config(argc, const_cast<char**>(argv)); },
      ConfigFileDoesNotExistException);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
