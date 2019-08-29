/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 20:10:17
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-29 18:30:22
*/

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

TEST(BasicTest, ParsingWithoutThrowingWhenHavingPlenoLFBoxButNoCodestream) {
  EXPECT_NO_THROW(auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin")); 
}


TEST(BasicTest, ParsingWithoutThrowingWhenHavingPlenoLFBoxWithSimpleCodestream) {
  EXPECT_NO_THROW(auto jpl_file = JPLFileFromStream(
       resources_path + "/boxes/jpl_file_lightfield_with_simple_codestream.bin")); 
}


TEST(BasicTest, ThreeBoxesDecodedInFileWithPlenoLFBox) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin");
  EXPECT_EQ(jpl_file.get_number_of_decoded_boxes(), 3); 
  // the 3 boxes are: signature, file header and jpeg pleno light field box
}


TEST(BasicTest, ThreeBoxesDecodedInFileWithPlenoLFBoxWithSimpleCodestream) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_simple_codestream.bin");
  EXPECT_EQ(jpl_file.get_number_of_decoded_boxes(), 3); 
  // the 3 boxes are: signature, file header and jpeg pleno light field box
}


TEST(BasicTest, OneCodestreamDecodedInFileWithPlenoLFBoxWithSimpleCodestream) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_simple_codestream.bin");
  EXPECT_EQ(jpl_file.number_of_codestreams(), 1); 
}


TEST(BasicTest, OneCodestreamDecodedInFileWithPlenoLFBoxButNoCodestream) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin");
  EXPECT_EQ(jpl_file.number_of_codestreams(),1); 
  // in this case the codestream is the jpeg pleno light field box (possible nomenclature problem)
}


TEST(BasicTest, TwoCodestreamsDecodedInFileWithPlenoLFBoxWithTwoSimpleCodestreams) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_two_simple_codestreams.bin");
  EXPECT_EQ(jpl_file.number_of_codestreams(),2); 
  // in this case the codestream is the jpeg pleno light field box (possible nomenclature problem)
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}