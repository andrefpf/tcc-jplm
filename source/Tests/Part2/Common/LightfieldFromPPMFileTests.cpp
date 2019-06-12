/*
* @Author: Ismael Seidel
* @Date:   2019-06-06 15:26:52
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-06-12 17:59:10
*/


#include <iostream>
#include "Lib/Part2/Common/LightfieldFromPPMFile.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


struct LFFromPPMSmallGreek : public testing::Test {
 protected:
  LightfieldDimension<std::size_t> size;
  LightfieldCoordinate<std::size_t> initial;
  LightfieldFromPPMFile<uint16_t> lightfield;

 public:
  LFFromPPMSmallGreek()
      : size(3, 3, 32, 32), initial(0, 0, 0, 0),
        lightfield({{resources_path + "/small_greek/"}, initial, size}){};
  ~LFFromPPMSmallGreek() = default;
};


TEST_F(LFFromPPMSmallGreek, HasCorrectWidth) {
  auto width = lightfield.get_width();
  EXPECT_EQ(width, 3);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectHeight) {
  auto height = lightfield.get_height();
  EXPECT_EQ(height, 3);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectViewWidth) {
  auto width = lightfield.get_views_width();
  EXPECT_EQ(width, 32);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectViewHeight) {
  auto height = lightfield.get_views_height();
  EXPECT_EQ(height, 32);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectViewBpp) {
  //notice that there is no bpp informed
  auto bpp = lightfield.get_views_bpp();
  EXPECT_EQ(bpp, 10);
}


TEST_F(LFFromPPMSmallGreek, HasCorrectDimensions) {
  auto dimensions = lightfield.get_dimensions();
  EXPECT_EQ(dimensions, size);
}


TEST_F(LFFromPPMSmallGreek, ZeroZeroImageIsEqual) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/small_greek/000_000.ppm"});
  auto variadic_image = file_original->read_full_image();
  auto image = std::move(std::get<1>(variadic_image));
  EXPECT_EQ(*image, lightfield.get_image_at({0, 0}));
}


TEST_F(LFFromPPMSmallGreek, ZeroZeroImageIsDifferentFromOneOneImage) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/small_greek/001_001.ppm"});
  auto variadic_image = file_original->read_full_image();
  auto image = std::move(std::get<1>(variadic_image));
  EXPECT_NE(*image, lightfield.get_image_at({0, 0}));
}


TEST_F(LFFromPPMSmallGreek, OneOneImageIsEqual) {
  auto file_original =
      PixelMapFileIO::open({resources_path + "/small_greek/001_001.ppm"});
  auto variadic_image = file_original->read_full_image();
  auto image = std::move(std::get<1>(variadic_image));
  EXPECT_EQ(*image, lightfield.get_image_at({1, 1}));
}


TEST_F(LFFromPPMSmallGreek, AllImagesAreEqualTheirExpectedImage) {
  PPM3CharViewToFilename filename_translator;
  for (auto t = 0; t < 3; ++t) {
    for (auto s = 0; s < 3; ++s) {
      auto file_original = PixelMapFileIO::open(
          {resources_path + "/small_greek/" +
              filename_translator.view_position_to_filename({t, s})});
      auto variadic_image = file_original->read_full_image();
      auto image = std::move(std::get<1>(variadic_image));
      EXPECT_EQ(*image, lightfield.get_image_at({t, s}));
    }
  }
}


TEST_F(LFFromPPMSmallGreek,
    TranposingTheIndicesGetsWrongImagesButTheMainDiagonal) {
  PPM3CharViewToFilename filename_translator;
  for (auto t = 0; t < 3; ++t) {
    for (auto s = 0; s < 3; ++s) {
      auto file_original = PixelMapFileIO::open(
          {resources_path + "/small_greek/" +
              filename_translator.view_position_to_filename({s, t})});
      auto variadic_image = file_original->read_full_image();
      auto image = std::move(std::get<1>(variadic_image));
      if (s == t) {
        EXPECT_EQ(*image, lightfield.get_image_at({t, s}));
      } else {
        EXPECT_NE(*image, lightfield.get_image_at({t, s}));
      }
    }
  }
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}
