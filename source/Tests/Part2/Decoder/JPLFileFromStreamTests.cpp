/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 20:10:17
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 20:21:54
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

TEST(BasicTest, TwoBoxesDecodedInSimplestFile) {
  auto jpl_file = JPLFileFromStream(
      resources_path + "/boxes/jpl_file_lightfield_with_no_codestream.bin");
  EXPECT_EQ(jpl_file.get_number_of_decoded_boxes(), 3);
}




int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}