/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 16:57:21
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 17:09:27
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";

TEST(BasicTest, ReadsAllDataFromStream) {
  std::string filename(
      resources_path + "/boxes/jpeg_pleno_box_with_no_codestream.bin");
  std::ifstream if_stream(filename, std::ifstream::binary);
  auto box =
      BoxParserRegistry::get_instance().parse<JpegPlenoLightFieldBox>(
          ManagedStream(if_stream, std::filesystem::file_size(filename)));

  EXPECT_EQ(if_stream.tellg(), std::filesystem::file_size(filename));
}
  

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}