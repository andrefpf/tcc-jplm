/*
* @Author: Ismael Seidel
* @Date:   2019-08-27 17:41:52
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 14:43:39
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