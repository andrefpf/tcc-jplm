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

/** \file     JpegPlenoLightFieldBoxParserTests.cpp
 *  \brief    Test of JpegPlenoLightFieldBoxParser.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-28
 */


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";

TEST(BasicTest, ReadsAllDataFromStream) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_box_with_no_codestream.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  EXPECT_EQ(if_stream.tellg(), std::filesystem::file_size(filename));
}
  

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}