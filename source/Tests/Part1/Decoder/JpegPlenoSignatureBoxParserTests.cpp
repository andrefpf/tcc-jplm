/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:21:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-16 19:38:51
*/

#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/Boxes/BoxParserRegistry.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "gtest/gtest.h"


std::string resources_path = "../resources";


int main(int argc, char *argv[]) {
  testing::InitGoogleTest(&argc, argv);
  //this is to enable ctest to run the test passing the path to the resources
  if (argc > 1) {
    resources_path = std::string(argv[1]);
  }

  std::ifstream if_stream(
      resources_path + "/boxes/signature_box.bin", std::ifstream::binary);
  auto managed_stream = ManagedStream(if_stream, 100);

  std::cout << if_stream.tellg() << std::endl;
  auto box = BoxParserRegistry::get_instance().parse<JpegPlenoSignatureBox>(
      managed_stream);
  std::cout << if_stream.tellg() << std::endl;
  std::cout << box->size() << std::endl;

  BoxParserRegistry::get_instance().parse(managed_stream);


  std::cout << "Main" << std::endl;

  return RUN_ALL_TESTS();
}