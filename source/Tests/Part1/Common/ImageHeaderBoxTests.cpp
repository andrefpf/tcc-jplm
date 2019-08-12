/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 12:47:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-12 12:50:11
*/

#include "Lib/Part1/Common/ImageHeaderBox.h"
#include "gtest/gtest.h"


TEST(ImageHeaderBoxBasicTest, ImageHeaderBoxInitializationDoesNotThrow) {
	EXPECT_NO_THROW(ImageHeaderBox());
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}