/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:21:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-19 15:57:11
*/

#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/BoxParserRegistry.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";

TEST(BasicFileTypeBoxTest, ThrowsIfWrongBox) {
  std::ifstream if_stream(
      resources_path + "/boxes/signature_box.bin", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 20);

 EXPECT_THROW(BoxParserRegistry::get_instance().parse<FileTypeBox>(
       managed_stream), BoxParserExceptions::WrongTBoxValueException);
}


TEST(BasicFileTypeBoxTest, ReadsAllDataFromStream) {
  std::ifstream if_stream(
      resources_path + "/boxes/file_type_box.bin", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 20);

  auto box = BoxParserRegistry::get_instance().parse<FileTypeBox>(
       managed_stream);

  EXPECT_EQ(managed_stream.tell(), 20);
}


TEST(BasicFileTypeBoxTest, ReadsOnlyTheAmountDefinedInLBoxDataFromStream) {
  std::ifstream if_stream(
      resources_path + "/boxes/file_type_box.bin", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 50);

  auto box = BoxParserRegistry::get_instance().parse<FileTypeBox>(
       managed_stream);

  EXPECT_EQ(managed_stream.tell(), 20);
}


TEST(BasicFileTypeBoxTest, IdentifiesCompatibilityList) {
  std::ifstream if_stream(
      resources_path + "/boxes/file_type_box.bin", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 20);

  auto box = BoxParserRegistry::get_instance().parse<FileTypeBox>(
       managed_stream);

  EXPECT_TRUE(box->is_compatible_with<JpegPlenoSignatureBox>());
}


TEST(BasicFileTypeBoxTest, IdentifiesIfNotInCompatibilityList) {
  std::ifstream if_stream(
      resources_path + "/boxes/file_type_box_not_compatible.bin", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 20);

  auto box = BoxParserRegistry::get_instance().parse<FileTypeBox>(
       managed_stream);

  EXPECT_FALSE(box->is_compatible_with<JpegPlenoSignatureBox>());
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}