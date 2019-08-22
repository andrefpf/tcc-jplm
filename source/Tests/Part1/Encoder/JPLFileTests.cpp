/*
* @Author: Ismael Seidel
* @Date:   2019-08-21 11:39:44
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-22 12:07:30
*/

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part1/Common/JPLFile.h"
#include "gtest/gtest.h"

std::string resources_path = "../resources";


struct JPLFileFixture : public testing::Test {
 protected:
  std::string output_filename;
  std::ofstream of_stream;

 public:
  JPLFileFixture() {
  }

  void set_filename(const std::string& filename) {
    output_filename = filename;
    of_stream.open(filename, std::ofstream::binary);
  }

  void flush() {
    of_stream.flush();
  }

  ~JPLFileFixture() {
    if (of_stream.is_open()) {
      of_stream.close();
    }
    namespace fs = std::filesystem;
    if (fs::exists(output_filename)) {
      fs::remove(output_filename);
    }
  }

  std::ofstream& cout() {
    return of_stream;
  }
};


TEST_F(JPLFileFixture, FileExists) {
  set_filename("temp.bin");
  EXPECT_TRUE(std::filesystem::exists("temp.bin"));
}


TEST_F(JPLFileFixture, CanPerformOperatorOutput) {
  std::string filename = "temp2.bin";
  set_filename(filename);

  auto jpl_file = JPLFile();
  EXPECT_EQ(std::filesystem::file_size(filename), 0);
  cout() << jpl_file;
  flush();
  EXPECT_EQ(std::filesystem::file_size(filename), 32);
}


int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  return RUN_ALL_TESTS();
}