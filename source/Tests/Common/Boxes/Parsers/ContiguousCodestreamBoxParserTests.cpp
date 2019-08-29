/*
* @Author: Ismael Seidel
* @Date:   2019-08-29 13:36:06
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-29 14:04:41
*/


#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Common/Boxes/Parsers/ContiguousCodestreamBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(BasicTest, ParsingABoxDoesNotThrow) {
  auto filename = std::string(
      resources_path + "/boxes/contiguous_codestream_box_with_small_25_42_54_252_code.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  EXPECT_NO_THROW(
      auto box =
          BoxParserRegistry::get_instance().parse<ContiguousCodestreamBox>(
              std::move(managed_stream)));
}


TEST(BasicTest, ReadsAllDataFromStream) {
  auto filename = std::string(
      resources_path + "/boxes/contiguous_codestream_box_with_small_code.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  auto box = BoxParserRegistry::get_instance().parse<ContiguousCodestreamBox>(
      std::move(managed_stream));
  EXPECT_EQ(if_stream.tellg(), 15);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}