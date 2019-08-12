/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 12:56:21
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-12 14:14:36
*/


#include "Lib/Part1/Common/ImageHeaderContents.h"
#include "gtest/gtest.h"


struct ImageHeaderContentsFixture : public testing::Test {
  uint32_t height = 1080;
  uint32_t width = 1920;
  uint16_t number_of_channels = 3;
  uint8_t bits_per_component = 10;
  CoderTypeC coder_type = CoderTypeC::JPEG_2000;
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


TEST_F(
    ImageHeaderContentsFixture, ImageHeaderContentsInitializationDoesNotThrow) {
  EXPECT_NO_THROW(ImageHeaderContents(height, width, number_of_channels,
      bits_per_component, coder_type, UnkC, IPR));
}


TEST_F(
    ImageHeaderContentsFixture, ImageHeaderContentsHasHeight) {
	// EXPECT_EQ(ImageHeaderContents->get_height(), height);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}