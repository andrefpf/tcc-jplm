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

/** \file     XLBoxTests.cpp
 *  \brief    Test of XLBox.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-29
 */

#include <filesystem>
#include <fstream>
#include <iostream>
#include "Lib/Common/Boxes/XLBox.h"
#include "gtest/gtest.h"


TEST(XLBoxBasicTest, XLBoxInitializationDoesNotThrow) {
  EXPECT_NO_THROW(XLBox(42));
}


TEST(XLBoxBasicTest, XLBoxWriteToStringOStream) {
  auto t_box = XLBox(42);
  std::ostringstream string_stream;
  string_stream << t_box.get_value();
  EXPECT_EQ(string_stream.str(), "42");
}


TEST(XLBoxBasicTest, XLBoxWriteToBinaryOStream) {
  namespace fs = std::filesystem;
  auto path = std::string(std::string(fs::temp_directory_path()) +
                          "/XLBoxWriteToBinaryOStream.test");
  if (fs::exists(path)) {
    fs::remove(path);
  }
  std::ofstream outfile(path.c_str(), std::ofstream::binary);
  auto t_box = XLBox(42);
  outfile << t_box;
  outfile.close();

  std::ifstream infile(path.c_str(), std::ofstream::binary);
  uint64_t test;
  infile.read(reinterpret_cast<char *>(&test), sizeof(uint64_t));
  test = BinaryTools::ensure_machines_endianess(test);

  EXPECT_EQ(test, 42);
}


TEST(XLBoxBasicTest, XLBoxContructionWithMinValueNotAllowedThrows) {
	EXPECT_THROW(XLBox(0), BoxExceptions::ValueNotAllowedException);
}


TEST(XLBoxBasicTest, XLBoxContructionWithMaxValueNotAllowedThrows) {
	EXPECT_THROW(XLBox(15), BoxExceptions::ValueNotAllowedException);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}