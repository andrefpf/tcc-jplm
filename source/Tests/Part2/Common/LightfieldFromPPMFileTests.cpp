/*
* @Author: Ismael Seidel
* @Date:   2019-06-06 15:26:52
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-06-10 12:28:07
*/



#include <iostream>
#include "Lib/Part2/Common/LightfieldFromPPMFile.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


TEST(ViewFromPPMFileIntantiation, Test) {
	LightfieldDimension<std::size_t> size(3,3,32,32);
	LightfieldCoordinate<std::size_t> initial(0,0,0,0);

	LightfieldIOConfiguration lfc(resources_path+"/small_greek/", initial, size);
	// EXPECT_EQ(lfc.get_size(), size);
	auto lf = LightfieldFromPPMFile<uint16_t>(lfc);
	// lf.
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
   if (argc > 1) {
    resources_path = std::string(argv[1]);
  }
  return RUN_ALL_TESTS();
}
