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

/** \file     FileTypeBoxTests.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-24
 */


#include <iostream>
#include "source/Lib/Part1/Common/Boxes/JpegPlenoFileTypeContents.h"
#include "source/Lib/Part1/Common/Boxes/FileTypeBox.h"
#include "gtest/gtest.h"


TEST(FileTypeBoxBasic, InitializationDoesNotThrow) {
  EXPECT_NO_THROW(
      auto file_type_box = FileTypeBox(FileTypeContents(0,0,{})));
}


struct FileTypeBoxContents : public testing::Test {
 protected:
  uint32_t brand = 0x0042;
  uint32_t minor_version=25;
  std::unique_ptr<FileTypeBox> file_type_box;
  FileTypeBoxContents() {
    // view.get_im
    file_type_box = std::make_unique<FileTypeBox>(FileTypeContents(brand, minor_version, {0x0004}));
  }

  const FileTypeContents& get_content_of_file() {
    return dynamic_cast<const FileTypeContents&>(file_type_box->get_ref_to_dbox_contents());
  }
};


TEST_F(FileTypeBoxContents, HoldsInitializedBrand) {
  EXPECT_EQ(get_content_of_file().get_brand(), brand);
}


TEST_F(FileTypeBoxContents, HoldsInitializedMinorVersion) {
  EXPECT_EQ(get_content_of_file().get_minor_version(), minor_version);
}


TEST_F(FileTypeBoxContents, HasCorrectSizeAfterCast) {
  EXPECT_EQ(get_content_of_file().size(), 4+4+4);
}


TEST_F(FileTypeBoxContents, CompatibilityCheckFailsForStdNotDedinedInCompatibilityList) {
  EXPECT_FALSE(get_content_of_file().is_the_file_compatible_with(0x0002));
}


TEST_F(FileTypeBoxContents, CompatibilityCheckSucceedsForStdDedinedInCompatibilityList) {
  EXPECT_TRUE(get_content_of_file().is_the_file_compatible_with(0x0004));
}


TEST_F(FileTypeBoxContents, FileTypeBoxHasCorrectSize) {
  EXPECT_EQ(file_type_box->size(), get_content_of_file().size()+4+4);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}