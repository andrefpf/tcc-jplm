/*
* @Author: Ismael Seidel
* @Date:   2019-08-19 16:20:08
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-20 14:58:24
*/


#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Decoder/JPLFileFromStream.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";

TEST(BasicTest, TwoBoxesDecodedInSimplestFile) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/signature_and_file_type_box.bin");
  EXPECT_EQ(jpl_file.get_number_of_decoded_boxes(), 2);
}


TEST(BasicTest, ThrowsIfFileIsSmallerThan20Bytes) {
  EXPECT_THROW(auto jpl_file = JPLFileFromStream(
                   resources_path + "/boxes/signature_box.bin"),
      JPLFileFromStreamExceptions::InvalidTooSmallFileException);
}


TEST(BasicTest, ThrowsIfNotBegginingWithJSignatureBox) {
  EXPECT_THROW(auto jpl_file = JPLFileFromStream(
                   resources_path + "/boxes/file_type_and_signature.bin"),
      BoxParserExceptions::WrongTBoxValueException);
}


TEST(BasicTest, ThrowsIfMoreThanOneFileTypeBox) {
  EXPECT_THROW(auto jpl_file = JPLFileFromStream(
                   resources_path +
                   "/boxes/signature_and_two_file_type_boxes.bin"),
      JPLFileFromStreamExceptions::MoreThanOneFileTypeBoxException);
}


TEST(BasicTest, ThrowsIfNotCompatibleWithPleno) {
  EXPECT_THROW(auto jpl_file = JPLFileFromStream(
                   resources_path +
                   "/boxes/signature_and_incompatible_file_type_box.bin"),
      JPLFileFromStreamExceptions::JpegPlenoNotInCompatibilityListException);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}