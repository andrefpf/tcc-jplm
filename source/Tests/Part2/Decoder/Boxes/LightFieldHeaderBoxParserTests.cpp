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

/** \file     LightFieldHeaderBoxParserTests.cpp
 *  \brief    Test of LightFieldHeaderBoxParser.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-27
 */


#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part2/Decoder/Boxes/LightFieldHeaderBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(BasicTest, ReadsAllDataFromStream) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  EXPECT_EQ(if_stream.tellg(), 30);
}


TEST(BasicTest, ContainsCorrectT) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(
      light_field_header_box_contents.get_light_field_dimension().get_t(), 13);
}


TEST(BasicTest, ContainsCorrectS) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(
      light_field_header_box_contents.get_light_field_dimension().get_s(), 31);
}


TEST(BasicTest, ContainsCorrectV) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(
      light_field_header_box_contents.get_light_field_dimension().get_v(), 512);
}


TEST(BasicTest, ContainsCorrectU) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(
      light_field_header_box_contents.get_light_field_dimension().get_u(), 215);
}


TEST(BasicTest, ContainsCorrectNC) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(light_field_header_box_contents.get_number_of_components(), 3);
}


TEST(BasicTest, ContainsCorrectBPC) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(light_field_header_box_contents.get_bits_per_component(), 10);
}


TEST(BasicTest, ContainsCorrectCompressionType) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(light_field_header_box_contents.get_compression_type(),
      CompressionTypeLightField::transform_mode);
}


TEST(BasicTest, ContainsCorrectUnkc) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(light_field_header_box_contents.get_color_space_unknown_flag(),
      ColourSpaceUnknownFlag::known);
}


TEST(BasicTest, ContainsCorrectIpr) {
  std::ifstream if_stream(
      resources_path +
          "/boxes/light_field_header_box_13x31x512x215x3x10x0x0x0.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 30);

  auto box = BoxParserRegistry::get_instance().parse<LightFieldHeaderBox>(
      std::move(managed_stream));

  const auto& light_field_header_box_contents =
      box->get_const_ref_to_contents();

  EXPECT_EQ(light_field_header_box_contents.get_intelectual_property_flag(),
      IntelectualPropertyFlag::has_no_rights_information);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}