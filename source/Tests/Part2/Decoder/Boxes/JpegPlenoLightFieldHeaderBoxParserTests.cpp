/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 13:02:39
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 16:06:41
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldHeaderBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";

TEST(BasicTest, ReadsAllDataFromStream) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          std::move(managed_stream));

  EXPECT_EQ(if_stream.tellg(), std::filesystem::file_size(filename));
}


TEST(BasicTest, HasColourSpecificationBox) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto colour_specs = jpeg_pleno_light_field_header_contents
                          .get_copy_of_colour_specification_boxes();

  EXPECT_EQ(colour_specs.size(), 1);
}


TEST(BasicTest, HasColourSpecificationBoxsRGB) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto colour_specs = jpeg_pleno_light_field_header_contents
                          .get_copy_of_colour_specification_boxes();


  EXPECT_EQ(
      colour_specs[0].get_const_ref_to_contents().get_colour_space_from_enum(),
      EnumCS::sRGB);
}


TEST(BasicTest, HasCorrectT) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(
      light_field_header_contents.get_light_field_dimension().get_t(), 13);
}


TEST(BasicTest, HasCorrectS) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(
      light_field_header_contents.get_light_field_dimension().get_s(), 31);
}


TEST(BasicTest, HasCorrectV) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(
      light_field_header_contents.get_light_field_dimension().get_v(), 512);
}


TEST(BasicTest, HasCorrectU) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(
      light_field_header_contents.get_light_field_dimension().get_u(), 215);
}


TEST(BasicTest, HasCorrectNc) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(light_field_header_contents.get_number_of_components(), 3);
}


TEST(BasicTest, HasCorrectBpc) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(light_field_header_contents.get_bits_per_component(), 10);
}


TEST(BasicTest, HasCorrectMode) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();
  auto light_field_header_contents =
      jpeg_pleno_light_field_header_contents.get_const_ref_to_lhdr()
          .get_const_ref_to_contents();


  EXPECT_EQ(light_field_header_contents.get_compression_type(),
      CompressionTypeLightField::transform_mode);
}


TEST(BasicTest, ThereIsNoChannelDefinitionBox) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();


  EXPECT_FALSE(jpeg_pleno_light_field_header_contents
                   .get_copy_of_channel_definition_box());
}


TEST(BasicTest, ThereIsNoBitsPerComponentBox) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_light_field_header_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);

  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldHeaderBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  const auto& jpeg_pleno_light_field_header_contents =
      box->get_const_ref_to_contents();


  EXPECT_FALSE(jpeg_pleno_light_field_header_contents
                   .get_copy_of_bits_per_component_box());
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}