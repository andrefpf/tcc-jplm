/*
* @Author: Ismael Seidel
* @Date:   2019-08-27 17:41:52
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-27 18:07:55
*/


#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part2/Decoder/Boxes/ProfileAndLevelBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(BasicTest, ReadsAllDataFromStream) {
  std::ifstream if_stream(
      resources_path + "/boxes/profile_and_level_box_4224.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 12);

  auto box = BoxParserRegistry::get_instance().parse<ProfileAndLevelBox>(
      managed_stream);

  EXPECT_EQ(managed_stream.tell(), 12);
}


TEST(BasicTest, ContainsCorrectProfile) {
  std::ifstream if_stream(
      resources_path + "/boxes/profile_and_level_box_4224.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 12);

  auto box = BoxParserRegistry::get_instance().parse<ProfileAndLevelBox>(
      managed_stream);

  const auto& profile_and_level_contents = box->get_const_ref_to_contents();

  EXPECT_EQ(profile_and_level_contents.get_ppih(), 42);
}


TEST(BasicTest, ContainsCorrectLevel) {
  std::ifstream if_stream(
      resources_path + "/boxes/profile_and_level_box_4224.bin",
      std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 12);

  auto box = BoxParserRegistry::get_instance().parse<ProfileAndLevelBox>(
      managed_stream);

  const auto& profile_and_level_contents = box->get_const_ref_to_contents();

  EXPECT_EQ(profile_and_level_contents.get_plev(), 24);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}