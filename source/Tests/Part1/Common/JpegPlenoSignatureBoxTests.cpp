/*
* @Author: Ismael Seidel
* @Date:   2019-07-23 17:50:25
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-07-23 18:09:08
*/

#include <iostream>
#include "Lib/Part1/Common/JpegPlenoSignatureBox.h"
#include "gtest/gtest.h"


TEST(JpegPlenoSignatureBoxBasic, Initialization) {
	EXPECT_NO_THROW(JpegPlenoSignatureBox());
}


TEST(JpegPlenoSignatureBoxBasic, CorrectLBoxValue) {
	auto pleno_signature_box = JpegPlenoSignatureBox();
	EXPECT_EQ(pleno_signature_box.get_lbox().get_value(), 12);
}


TEST(JpegPlenoSignatureBoxBasic, XLBoxDoNotExist) {
	auto pleno_signature_box = JpegPlenoSignatureBox();
	EXPECT_FALSE(pleno_signature_box.get_xlbox());
}


TEST(JpegPlenoSignatureBoxBasic, CorrectTBoxValue) {
	auto pleno_signature_box = JpegPlenoSignatureBox();
	EXPECT_EQ(pleno_signature_box.get_tbox().get_value(), 0x6a706c20);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}