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
 *  \brief    Test of JPLFile.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-22
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Common/JPLFile.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";


struct JPLFileFixture : public testing::Test {
 protected:
  std::string output_filename;
  std::ofstream of_stream;

 public:
  JPLFileFixture() {
  }

  void set_filename(const std::string& filename) {
    output_filename = filename;
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


TEST_F(JPLFileFixture, FileExists) {
  set_filename("temp.bin");
  EXPECT_TRUE(std::filesystem::exists("temp.bin"));
}


TEST_F(JPLFileFixture, CanPerformOperatorOutput) {
  std::string filename = "temp2.bin";
  set_filename(filename);

  auto jpl_file = JPLFile();
  EXPECT_EQ(std::filesystem::file_size(filename), 0);
  cout() << jpl_file;
  flush();
  EXPECT_EQ(std::filesystem::file_size(filename), 32);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}