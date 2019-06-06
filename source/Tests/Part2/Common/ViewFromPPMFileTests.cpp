/*
* @Author: Ismael Seidel
* @Date:   2019-06-06 15:26:52
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-06-06 16:02:50
*/



#include <iostream>
#include "Lib/Part2/Common/ViewFromPPMFile.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(ViewFromPPMFileIntantiation, Test) {
	ViewFromPPMFile<uint16_t>(resources_path+"/small_greek/", {0,0});
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
   if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}
