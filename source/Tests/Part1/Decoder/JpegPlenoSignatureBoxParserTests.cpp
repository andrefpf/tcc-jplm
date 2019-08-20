/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:21:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-20 15:14:05
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/BoxParserRegistry.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";


TEST(BasicTest, ParseWithoutThrowIfCorrectBox) {
  auto filename = std::string(resources_path + "/boxes/signature_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  EXPECT_NO_THROW(auto box = BoxParserRegistry::get_instance().parse<JpegPlenoSignatureBox>(
        managed_stream));
}


TEST(BasicTest, ThrowsIfParsingIncorrectBoxType) {
  auto filename = std::string(resources_path + "/boxes/file_type_box.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  EXPECT_THROW(auto box = BoxParserRegistry::get_instance().parse<JpegPlenoSignatureBox>(
        managed_stream), BoxParserExceptions::WrongTBoxValueException);
}


TEST(BasicTest, ThrowsIfSignatureBoxDataIsCorrupted) {
  auto filename = std::string(resources_path + "/boxes/signature_box_invalid_dbox.bin");
  //there is one bit fliped...
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto managed_stream =
      ManagedStream(if_stream, std::filesystem::file_size(filename));
  EXPECT_THROW(auto box = BoxParserRegistry::get_instance().parse<JpegPlenoSignatureBox>(
        managed_stream), JpegPlenoSignatureBoxParserExceptions::InvalidJpegPlenoSignatureBox);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}