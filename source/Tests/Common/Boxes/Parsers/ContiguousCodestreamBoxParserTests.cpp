/*
* @Author: Ismael Seidel
* @Date:   2019-08-29 13:36:06
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-09-03 14:33:50
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
      resources_path +
      "/boxes/contiguous_codestream_box_with_small_25_42_54_252_code.bin");
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
      resources_path +
      "/boxes/contiguous_codestream_box_with_small_25_42_54_252_code.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  auto box = BoxParserRegistry::get_instance().parse<ContiguousCodestreamBox>(
      std::move(managed_stream));
  EXPECT_EQ(if_stream.tellg(), std::filesystem::file_size(filename));
}


TEST(BasicTest, CodeHasExpectedSize) {
  auto filename = std::string(
      resources_path +
      "/boxes/contiguous_codestream_box_with_small_25_42_54_252_code.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  auto box = BoxParserRegistry::get_instance().parse<ContiguousCodestreamBox>(
      std::move(managed_stream));

  const auto& contiguous_codestream_box_contents = box->get_ref_to_contents();
  const auto& contiguous_codestream_box_code =
      contiguous_codestream_box_contents.get_ref_to_code();

  EXPECT_EQ(contiguous_codestream_box_code.size(), 4);
}


TEST(BasicTest, ReadsAllExpectedCode) {
  auto filename = std::string(
      resources_path +
      "/boxes/contiguous_codestream_box_with_small_25_42_54_252_code.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  auto box = BoxParserRegistry::get_instance().parse<ContiguousCodestreamBox>(
      std::move(managed_stream));

  const auto& contiguous_codestream_box_code =
      box->get_ref_to_contents().get_ref_to_code();

  EXPECT_EQ(contiguous_codestream_box_code.get_byte_at(0), std::byte{25});
  EXPECT_EQ(contiguous_codestream_box_code.get_byte_at(1), std::byte{42});
  EXPECT_EQ(contiguous_codestream_box_code.get_byte_at(2), std::byte{54});
  EXPECT_EQ(contiguous_codestream_box_code.get_byte_at(3), std::byte{252});
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}