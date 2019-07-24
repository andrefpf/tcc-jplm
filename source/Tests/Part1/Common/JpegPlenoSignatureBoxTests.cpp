/*
* @Author: Ismael Seidel
* @Date:   2019-07-23 17:50:25
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-07-24 10:26:22
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


TEST(JpegPlenoSignatureBoxBasic, TBoxValueNotOne) {
	auto pleno_signature_box = JpegPlenoSignatureBox();
	EXPECT_NE(pleno_signature_box.get_tbox().get_value(), 1);
}


TEST(JpegPlenoSignatureBoxBasic, CorrectTBoxValue) {
	auto pleno_signature_box = JpegPlenoSignatureBox();
	EXPECT_EQ(pleno_signature_box.get_tbox().get_value(), 0x6a706c20);
}


TEST(JpegPlenoSignatureBoxBasic, CorrectDBoxValue) {
	auto pleno_signature_box = JpegPlenoSignatureBox();
	auto dbox = pleno_signature_box.get_dbox();
	auto contents = dbox->get_ref_to_contents();
	auto& vec = std::any_cast<std::vector<unsigned char>&>(contents);
	EXPECT_EQ(vec.at(0), 0x0d);
	EXPECT_EQ(vec.at(1), 0x0a);
	EXPECT_EQ(vec.at(2), 0x87);
	EXPECT_EQ(vec.at(3), 0x0a);
}


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}