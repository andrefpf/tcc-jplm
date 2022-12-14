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

/** \file     ColourComponentScalingMarkerSegmentTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-01-06
 */


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCodeInMemory.h"
#include "Lib/Part2/Common/TransformMode/Block4D.h"
#include "Lib/Part2/Common/TransformMode/ColourComponentScalingMarkerSegment.h"
#include "Lib/Part2/Decoder/TransformMode/ColourComponentScalingMarkerSegmentParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(BasicTest, ColourComponentScalingMarkerSegmentConstructorDoesNotThrow) {
  EXPECT_NO_THROW(auto colour_component_scaling_marker_segment =
                      ColourComponentScalingMarkerSegment(false, 2, 63490));
}


TEST(BasicTest, ColourComponentScalingMarkerGivesCorrectColourComponentIndex) {
  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegment(false, 2, 63490);
  auto colour_component_index =
      colour_component_scaling_marker_segment.get_colour_component_index();
  EXPECT_EQ(std::get<0>(colour_component_index), 2);
}


TEST(BasicTest,
    ColourComponentScalingMarkerGivesCorrectAlternativeForTheNumberOfColourComponents) {
  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegment(false, 2, 63490);
  EXPECT_FALSE(colour_component_scaling_marker_segment
                   .has_more_than_256_colour_components());
}


TEST(BasicTest, ColourComponentScalingMarkerGivesCorrectExponent) {
  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegment(false, 2, 63490);
  auto exponent = colour_component_scaling_marker_segment.get_exponent();
  EXPECT_EQ(exponent, 0x1F);
}


TEST(BasicTest, ColourComponentScalingMarkerGivesCorrectMantissa) {
  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegment(false, 2, 63490);
  auto mantissa = colour_component_scaling_marker_segment.get_mantissa();
  EXPECT_EQ(mantissa, 2);
}


TEST(BasicTest, ColourComponentScalingMarkerGivesCorrectScaling) {
  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegment(false, 2, 63490);
  auto scaling_factor =
      colour_component_scaling_marker_segment.get_scaling_factor();
  EXPECT_EQ(scaling_factor, 65536);
}


TEST(FileTest, FileBeginsWithMarkerPrefix) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  EXPECT_EQ(managed_stream.get_byte(), std::byte(0xFF));
}


TEST(FileTest, FileBeginsWithColourComponentScalingMarkerPrefix) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  EXPECT_EQ(managed_stream.get_byte(), std::byte(0xFF));
  EXPECT_EQ(managed_stream.get_byte(), std::byte(0xA2));
}


TEST(ContigousCodestreamAndParserTest, FileBeginsWithMarkerPrefix) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
}


TEST(ContigousCodestreamAndParserTest,
    FileBeginsWithColourComponentScalingMarkerPrefix) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xA2));
}


TEST(ContigousCodestreamAndParserTest, ParserDoesNotThrow) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xA2));


  EXPECT_NO_THROW(auto colour_component_scaling_marker_segment =
                      ColourComponentScalingMarkerSegmentParser::
                          get_colour_component_scaling_marker_segment(
                              contiguous_codestream));
}


TEST(ContigousCodestreamAndParserTest,
    ParserCorrectAlternativeForTheNumberOfColourComponents) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xA2));


  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegmentParser::
          get_colour_component_scaling_marker_segment(contiguous_codestream);
  EXPECT_FALSE(colour_component_scaling_marker_segment
                   .has_more_than_256_colour_components());
}


TEST(ContigousCodestreamAndParserTest,
    ParserGivesCorrectExponent) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xA2));


  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegmentParser::
          get_colour_component_scaling_marker_segment(contiguous_codestream);
  auto exponent = colour_component_scaling_marker_segment.get_exponent();
  EXPECT_EQ(exponent, 0x1F);
}


TEST(ContigousCodestreamAndParserTest,
    ParserGivesCorrectMantissa) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xA2));


  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegmentParser::
          get_colour_component_scaling_marker_segment(contiguous_codestream);
  auto mantissa = colour_component_scaling_marker_segment.get_mantissa();
  EXPECT_EQ(mantissa, 2);
}


TEST(ContigousCodestreamAndParserTest,
    ParserGivesCorrectScalingFactor) {
  std::string filename(
      resources_path + "/markers/colour_component_scaling_marker.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 8);
  auto contiguous_codestream =
      ContiguousCodestreamCodeInMemory(managed_stream.get_n_bytes(8));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xFF));
  EXPECT_EQ(contiguous_codestream.get_next_byte(), std::byte(0xA2));


  auto colour_component_scaling_marker_segment =
      ColourComponentScalingMarkerSegmentParser::
          get_colour_component_scaling_marker_segment(contiguous_codestream);
  auto scaling_factor =
      colour_component_scaling_marker_segment.get_scaling_factor();
  EXPECT_EQ(scaling_factor, 65536);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}