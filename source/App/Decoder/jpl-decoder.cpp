#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part2/Encoder/JPLM4DTransformModeLightFieldEncoder.h"


int main(int argc, char const* argv[]) {
  auto configuration = JPLMConfigurationFactory::get_configuration(argc, argv);
  auto decoder = JPLMCodecFactory::get_decoder(std::move(configuration));
  decoder.run();
  exit(EXIT_SUCCESS);
}