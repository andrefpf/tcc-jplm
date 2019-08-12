/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 12:56:21
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-12 14:32:06
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


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}