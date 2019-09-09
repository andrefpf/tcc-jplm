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

/** \file     ImageHeaderContentsTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-12
 */


#include "Lib/Common/Boxes/Generic/ImageHeaderContents.h"
#include "gtest/gtest.h"


struct ImageHeaderContentsFixture : public testing::Test {
  uint32_t height = 1080;
  uint32_t width = 1920;
  uint16_t number_of_channels = 3;
  uint8_t bits_per_component = 10;
  CompressionTypeImage coder_type = CompressionTypeImage::JPEG_2000;
  uint8_t UnkC = 0;
  uint8_t IPR = 0;
  std::unique_ptr<ImageHeaderContents> image_header_contents;

  ImageHeaderContentsFixture()
      : image_header_contents(std::make_unique<ImageHeaderContents>(height,
            width, number_of_channels, bits_per_component, coder_type, UnkC,
            IPR)) {
  }
  ~ImageHeaderContentsFixture() = default;
};


TEST_F(ImageHeaderContentsFixture,
    ImageHeaderContentsInitializationWithZeroHeightThrows) {
  EXPECT_THROW(ImageHeaderContents(0, width, number_of_channels,
                   bits_per_component, coder_type, UnkC, IPR),
      ImageHeaderBoxExceptions::InvalidSizeException);
}


TEST_F(ImageHeaderContentsFixture,
    ImageHeaderContentsInitializationWithZeroWidthThrows) {
  EXPECT_THROW(ImageHeaderContents(height, 0, number_of_channels,
                   bits_per_component, coder_type, UnkC, IPR),
      ImageHeaderBoxExceptions::InvalidSizeException);
}


TEST_F(ImageHeaderContentsFixture,
    ImageHeaderContentsInitializationWithZeroNumberOfChannelsThrows) {
  EXPECT_THROW(ImageHeaderContents(
                   height, width, 0, bits_per_component, coder_type, UnkC, IPR),
      ImageHeaderBoxExceptions::InvalidSizeException);
}


TEST_F(ImageHeaderContentsFixture,
    ImageHeaderContentsInitializationWithZeroNumberOfBPCThrows) {
  EXPECT_THROW(ImageHeaderContents(
                   height, width, number_of_channels, 0, coder_type, UnkC, IPR),
      ImageHeaderBoxExceptions::InvalidSizeException);
}


TEST_F(
    ImageHeaderContentsFixture, ImageHeaderContentsInitializationDoesNotThrow) {
  EXPECT_NO_THROW(ImageHeaderContents(height, width, number_of_channels,
      bits_per_component, coder_type, UnkC, IPR));
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasHeight) {
  EXPECT_EQ(image_header_contents->get_height(), height);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasWidth) {
  EXPECT_EQ(image_header_contents->get_width(), width);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasNumberOfChannels) {
  EXPECT_EQ(image_header_contents->get_nc(), number_of_channels);
}


TEST_F(ImageHeaderContentsFixture,
    ImageHeaderContentsHasNumberOfChannelsInterface) {
  EXPECT_EQ(
      image_header_contents->get_number_of_channels(), number_of_channels);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasBpc) {
  EXPECT_EQ(image_header_contents->get_bpc(), bits_per_component);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasBpcInterface) {
  EXPECT_EQ(
      image_header_contents->get_bits_per_component(), bits_per_component);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasCoderType) {
  EXPECT_EQ(image_header_contents->get_c(), coder_type);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsHasCoderTypeInterface) {
  EXPECT_EQ(image_header_contents->get_coder_type(), coder_type);
}


TEST_F(ImageHeaderContentsFixture, ImageHeaderContentsUnknownColorCheck) {
  EXPECT_TRUE(image_header_contents->has_known_color_space());
}


TEST_F(
    ImageHeaderContentsFixture, ImageHeaderContentsIntellectualPropertyCheck) {
  EXPECT_FALSE(image_header_contents->has_intellectual_property());
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}