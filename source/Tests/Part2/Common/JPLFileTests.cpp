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

/** \file     JPLFileTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-27
 */


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Common/JPLFile.h"
#include "Lib/Part2/Common/Boxes/JpegPlenoLightFieldBox.h"
#include "gtest/gtest.h"

struct JPLFileFixture : public testing::Test {
 protected:
  std::string output_filename;
  std::ofstream of_stream;

 public:
  JPLFileFixture() {
  }

  void set_filename(const std::string& filename) {
    output_filename = filename;
    if (of_stream.is_open()) {
      of_stream.close();
    }
    of_stream.open(filename, std::ofstream::binary);
  }

  void flush() {
    of_stream.flush();
  }

  ~JPLFileFixture() {
    if (of_stream.is_open()) {
      of_stream.close();
    }
    namespace fs = std::filesystem;
    if (fs::exists(output_filename)) {
      fs::remove(output_filename);
    }
  }

  std::ofstream& cout() {
    return of_stream;
  }
};


TEST_F(JPLFileFixture, Initialization) {
  auto lf_header_contents = LightFieldHeaderContents(
      {1, 2, 3, 42}, 3, 8, CompressionTypeLightField::transform_mode);
  auto lf_header_box = LightFieldHeaderBox(lf_header_contents);
  std::vector<std::unique_ptr<ColourSpecificationBox>> colr;
  colr.emplace_back(std::make_unique<ColourSpecificationBox>());

  auto jpeg_pleno_light_field_header_contents =
      JpegPlenoLightFieldHeaderContents(lf_header_contents, colr);


  auto jpeg_pleno_light_field_header_box =
      std::make_unique<JpegPlenoLightFieldHeaderBox>(
          jpeg_pleno_light_field_header_contents);


  auto profile_and_level_box = std::make_unique<ProfileAndLevelBox>(
      ProfileAndLevelContents(11, 12));  // not sure which values to use...


  auto jpeg_pleno_light_field_contents =
      JpegPlenoLightFieldContents(std::move(profile_and_level_box),
          std::move(jpeg_pleno_light_field_header_box));


  auto jpeg_pleno_light_field_box = std::make_unique<JpegPlenoLightFieldBox>(
      std::move(jpeg_pleno_light_field_contents));

  auto jpl_file = JPLFile();
  jpl_file.add_codestream_box(std::move(jpeg_pleno_light_field_box));

  set_filename("tempJPLFile.bin");

  EXPECT_NO_THROW(cout() << jpl_file);

}



int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}