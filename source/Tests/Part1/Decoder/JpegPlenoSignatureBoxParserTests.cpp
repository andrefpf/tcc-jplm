/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:21:01
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-15 17:42:38
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

  std::optional<std::unique_ptr<Box>> box =
      BoxParserRegistry::get_instance().parse(
          JpegPlenoSignatureBox::id, managed_stream);


  BoxParserRegistry::get_instance().parse(0x000020, managed_stream);

  BoxParserRegistry::get_instance().parse(JpegPlenoSignatureBox::id, managed_stream);


  std::cout << "Main" << std::endl;

  return RUN_ALL_TESTS();
}