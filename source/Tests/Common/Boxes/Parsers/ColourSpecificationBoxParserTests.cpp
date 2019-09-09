/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 11:12:30
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-09-03 14:33:21
*/

#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Common/Boxes/Parsers/ColourSpecificationBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(BasicTest, ReadsAllDataFromStream) {
  std::ifstream if_stream(
      resources_path + "/boxes/colour_specification_box_meth1x0x0xsRGB.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 15);

  auto box = BoxParserRegistry::get_instance().parse<ColourSpecificationBox>(
      std::move(managed_stream));

  EXPECT_EQ(if_stream.tellg(), 15);
}


TEST(BasicTest, MethodIs1) {
  std::ifstream if_stream(
      resources_path + "/boxes/colour_specification_box_meth1x0x0xsRGB.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 15);

  auto box = BoxParserRegistry::get_instance().parse<ColourSpecificationBox>(
      std::move(managed_stream));

  const auto& colour_specification_box_contents =
      box->get_ref_to_contents();

  EXPECT_EQ(colour_specification_box_contents.get_specification_method(), 1);
}


TEST(BasicTest, PrecedenceIs0) {
  std::ifstream if_stream(
      resources_path + "/boxes/colour_specification_box_meth1x0x0xsRGB.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 15);

  auto box = BoxParserRegistry::get_instance().parse<ColourSpecificationBox>(
      std::move(managed_stream));

  const auto& colour_specification_box_contents =
      box->get_ref_to_contents();

  EXPECT_EQ(colour_specification_box_contents.get_precedence(), 0);
}


TEST(BasicTest, ApproximationIs0) {
  std::ifstream if_stream(
      resources_path + "/boxes/colour_specification_box_meth1x0x0xsRGB.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 15);

  auto box = BoxParserRegistry::get_instance().parse<ColourSpecificationBox>(
      std::move(managed_stream));

  const auto& colour_specification_box_contents =
      box->get_ref_to_contents();

  EXPECT_EQ(
      colour_specification_box_contents.get_colour_space_approximation(), 0);
}


TEST(BasicTest, EnumCSIsAvailable) {
  std::ifstream if_stream(
      resources_path + "/boxes/colour_specification_box_meth1x0x0xsRGB.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 15);

  auto box = BoxParserRegistry::get_instance().parse<ColourSpecificationBox>(
      std::move(managed_stream));

  const auto& colour_specification_box_contents =
      box->get_ref_to_contents();

  //get_colour_space_from_enum returns an optional
  EXPECT_TRUE(colour_specification_box_contents.get_colour_space_from_enum());
}


TEST(BasicTest, EnumCSIssRGB) {
  std::ifstream if_stream(
      resources_path + "/boxes/colour_specification_box_meth1x0x0xsRGB.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 15);

  auto box = BoxParserRegistry::get_instance().parse<ColourSpecificationBox>(
      std::move(managed_stream));

  const auto& colour_specification_box_contents =
      box->get_ref_to_contents();

  //get_colour_space_from_enum returns an optional
  EXPECT_EQ(*(colour_specification_box_contents.get_colour_space_from_enum()),
      EnumCS::sRGB);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}