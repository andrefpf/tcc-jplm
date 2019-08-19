/*
* @Author: Ismael Seidel
* @Date:   2019-08-19 16:20:08
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-19 17:22:01
*/


#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Decoder/JPLFileFromStream.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  auto jpl_file = JPLFileFromStream(resources_path + "/boxes/signature_and_file_type_box.bin");

  return RUN_ALL_TESTS();
}