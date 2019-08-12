/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 12:47:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-12 15:09:03
*/

#include "Lib/Part1/Common/ImageHeaderBox.h"
#include "gtest/gtest.h"


TEST(ImageHeaderBoxBasicTest, ImageHeaderBoxInitializationDoesNotThrow) {
  uint32_t height = 1080;
  uint32_t width = 1920;
  uint16_t number_of_channels = 3;
  uint8_t bits_per_component = 10;
  CoderTypeC coder_type = CoderTypeC::JPEG_2000;
  uint8_t UnkC = 0;
  uint8_t IPR = 0;

  EXPECT_NO_THROW(ImageHeaderBox(ImageHeaderContents(height, width, number_of_channels,
      bits_per_component, coder_type, UnkC, IPR)));
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}