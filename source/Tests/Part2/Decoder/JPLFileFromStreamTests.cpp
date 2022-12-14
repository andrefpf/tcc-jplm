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

/** \file     JPLFileFromStreamTests.cpp
 *  \brief    Test of JPLFileFromStream.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-28
 */


#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Decoder/JPLFileFromStream.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";

TEST(BasicTest, ParsingWithoutThrowingWhenHavingPlenoLFBoxButNoCodestream) {
  EXPECT_NO_THROW(auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin")); 
}


TEST(BasicTest, ParsingWithoutThrowingWhenHavingPlenoLFBoxWithSimpleCodestream) {
  EXPECT_NO_THROW(auto jpl_file = JPLFileFromStream(
       resources_path + "/boxes/jpl_file_lightfield_with_simple_codestream.bin")); 
}


TEST(BasicTest, ThreeBoxesDecodedInFileWithPlenoLFBox) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin");
  EXPECT_EQ(jpl_file.get_number_of_decoded_boxes(), 3); 
  // the 3 boxes are: signature, file header and jpeg pleno light field box
}


TEST(BasicTest, ThreeBoxesDecodedInFileWithPlenoLFBoxWithSimpleCodestream) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_simple_codestream.bin");
  EXPECT_EQ(jpl_file.get_number_of_decoded_boxes(), 3); 
  // the 3 boxes are: signature, file header and jpeg pleno light field box
}


TEST(BasicTest, OneCodestreamDecodedInFileWithPlenoLFBoxWithSimpleCodestream) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_simple_codestream.bin");
  EXPECT_EQ(jpl_file.number_of_codestreams(), 1); 
}


TEST(BasicTest, OneCodestreamDecodedInFileWithPlenoLFBoxButNoCodestream) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin");
  EXPECT_EQ(jpl_file.number_of_codestreams(),1); 
  // in this case the codestream is the jpeg pleno light field box (possible nomenclature problem)
}


TEST(BasicTest, TwoCodestreamsDecodedInFileWithPlenoLFBoxWithTwoSimpleCodestreams) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_two_simple_codestreams.bin");
  EXPECT_EQ(jpl_file.number_of_codestreams(),2); 
  // in this case the codestream is the jpeg pleno light field box (possible nomenclature problem)
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}