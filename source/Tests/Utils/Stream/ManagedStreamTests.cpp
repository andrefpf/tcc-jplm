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

/** \file     ManagedStreamTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-12
 */

#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(ManagedStreamBasics, ManagedStreamDoesNotTestForEOFDuringConstruction) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);

  EXPECT_NO_THROW(ManagedStream(if_stream, 10000));
}


TEST(ManagedStreamBasics, ManagedStreamRewindGoesToInitialLocation) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 10000);
  auto position_before = if_stream.tellg();
  managed_stream.rewind();
  auto position_after = if_stream.tellg();
  EXPECT_EQ(position_before, position_after);
}


TEST(ManagedStreamBasics, ManagedStreamRewindGoesToFinalLocation) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);

  auto managed_stream = ManagedStream(if_stream, 2);


  managed_stream.forward();
  auto position_after = if_stream.tellg();
  EXPECT_EQ(position_after, 2);
}

//! \todo Check to see if the ManagedStreamThrowsIfPassedEOFInForward test (or similar) is necessary
// TEST(ManagedStreamBasics, ManagedStreamThrowsIfPassedEOFInForward) {
//   std::ifstream if_stream(
//       resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
//   auto managed_stream = ManagedStream(if_stream, 10000);

//   EXPECT_THROW(managed_stream.forward(),
//       ManagedStreamExceptions::TryingToAccessBeyondEOFException);
// }


TEST(ManagedStreamBasics, ManagedStreamThrowsIfConstructedWithClosedStream) {
  std::ifstream if_stream;
  EXPECT_THROW(ManagedStream(if_stream, 2),
      ManagedStreamExceptions::ClosedStreamException);
}


TEST(ManagedStreamBasics,
    ManagedStreamThrowsIfConstructedWithFinalPositionSmallerThanInitial) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  EXPECT_THROW(ManagedStream(if_stream, 10, 5),
      ManagedStreamExceptions::FinalPositioSmallerThanInitialException);
}


TEST(ManagedStreamBasics,
    ManagedStreamIfConstructedWithEqualInitialAndFinalPosition) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  uint64_t initial_and_final = 5;
  EXPECT_THROW(ManagedStream(if_stream, initial_and_final, initial_and_final),
      ManagedStreamExceptions::InvalidEqualInitialAndFinalPositionException);
}


TEST(ManagedStreamBasics, ManagedStreamIfConstructedWithZeroOffset) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  EXPECT_THROW(ManagedStream(if_stream, 0),
      ManagedStreamExceptions::InvalidEqualInitialAndFinalPositionException);
}


TEST(ManagedStreamBasics, ManagedStreamCanProvideASubmanagedStream) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1);
  EXPECT_NO_THROW(managed_stream.get_sub_managed_stream(1));
}


TEST(ManagedStreamBasics,
    ManagedStreamSubmanagedStreamThrowsIfLargerThenSource) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1);
  EXPECT_THROW(managed_stream.get_sub_managed_stream(2),
      ManagedStreamExceptions::InvalidIndexForSubManagedStreamException);
}


TEST(ManagedStreamBasics, ManagedStreamCanProvideAByte) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1);
  EXPECT_NO_THROW(managed_stream.get_byte());
}


TEST(ManagedStreamBasics,
    ManagedStreamProvidingAByteChangesBy1ThePositionOfIfStream) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1);
  auto initial_stream_position = if_stream.tellg();
  [[maybe_unused]] auto byte = managed_stream.get_byte();
  EXPECT_EQ(static_cast<int64_t>(if_stream.tellg()),
      static_cast<int64_t>(initial_stream_position) + 1);
}


TEST(ManagedStreamBasics,
    ManagedStreamTryingToReadAByteAfterLimitedEndThrowsException) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1);
  [[maybe_unused]] auto byte = managed_stream.get_byte();
  EXPECT_THROW(
      managed_stream.get_byte(), ManagedStreamExceptions::OutOfBoundsException);
}


TEST(ManagedStreamBasics,
    ManagedStreamTryingToReadAByteBeforeLimitedBegginingThrowsException) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 5, 10);
  if_stream.seekg(0, std::ios_base::beg);
  EXPECT_THROW(
      managed_stream.get_byte(), ManagedStreamExceptions::OutOfBoundsException);
}


TEST(ManagedStreamBasics, ManagedStreamCanSeekInsideIntervalMinimum) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);
  auto current_position = static_cast<int64_t>(if_stream.tellg());

  EXPECT_EQ(managed_stream.seek(0).get_current_pos(), current_position);
}

TEST(ManagedStreamBasics, ManagedStreamCanSeekInsideIntervalMaximum) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  uint64_t offset = 10;
  auto managed_stream = ManagedStream(if_stream, 0, offset);
  auto current_position = static_cast<int64_t>(if_stream.tellg());

  EXPECT_EQ(
      managed_stream.seek(10).get_current_pos(), current_position + offset);
}


TEST(ManagedStreamBasics, ManagedStreamSeekInsideIntervalBeforeMinimumThrows) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);

  EXPECT_THROW(managed_stream.seek(-1),
      ManagedStreamExceptions::SeekBeforeInitialPositionException);
}


TEST(ManagedStreamBasics, ManagedStreamSeekInsideIntervalAfterMaximumThrows) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);

  EXPECT_THROW(managed_stream.seek(11),
      ManagedStreamExceptions::SeekAfterFinalPositionException);
}


TEST(ManagedStreamBasics,
    ManagedStreamSeekInsideIntervalRelativeToCurThrowsIfBefore) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1, 10);

  if_stream.seekg(0, std::ios_base::beg);

  EXPECT_THROW(managed_stream.seek(0, std::ios_base::cur),
      ManagedStreamExceptions::SeekBeforeInitialPositionException);
}


TEST(ManagedStreamBasics, ManagedStreamSeekInsideIntervalRelativeToCur) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1, 10);

  if_stream.seekg(0, std::ios_base::beg);

  EXPECT_EQ(managed_stream.seek(10, std::ios_base::cur).get_current_pos(), 10);
}


TEST(ManagedStreamBasics, ManagedStreamSeekInsideIntervalRelativeToCurSeek0) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1, 10);

  if_stream.seekg(10, std::ios_base::beg);

  EXPECT_EQ(managed_stream.seek(0, std::ios_base::cur).get_current_pos(), 10);
}


TEST(ManagedStreamBasics,
    ManagedStreamSeekInsideIntervalRelativeToCurThrowsAfter) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 1, 10);

  if_stream.seekg(10, std::ios_base::beg);

  EXPECT_THROW(managed_stream.seek(1, std::ios_base::cur),
      ManagedStreamExceptions::SeekAfterFinalPositionException);
}


TEST(ManagedStreamBasics, ManagedStreamCanTellPosition) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);

  auto position = if_stream.tellg();

  EXPECT_EQ(managed_stream.tell(), position);
}


TEST(ManagedStreamBasics, ManagedStreamTellPositionEqualsCurrentPosition) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);

  EXPECT_EQ(managed_stream.tell(), managed_stream.get_current_pos());
}


TEST(ManagedStreamBasics, ManagedStreamIsValidAfterConstruction) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);

  EXPECT_TRUE(managed_stream.is_valid());
}


TEST(ManagedStreamBasics, ManagedStreamIsValidAfterConstructionSeekToNearEnd) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);
  if_stream.seekg(9, std::ios_base::beg);
  EXPECT_TRUE(managed_stream.is_valid());
}


TEST(ManagedStreamBasics, ManagedStreamIsNotValidAfterSeekBeyondEnd) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 0, 10);
  if_stream.seekg(10, std::ios_base::beg);
  EXPECT_FALSE(managed_stream.is_valid());
}


TEST(ManagedStreamBasics, ManagedStreamGetsTheSize) {
  std::ifstream if_stream(
      resources_path + "/rgb_pattern/pattern.ppm", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 2, 10);

  EXPECT_EQ(managed_stream.get_length(), 8);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}