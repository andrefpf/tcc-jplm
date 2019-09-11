#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/JPLMConfigurationFactory.h"
#include "Lib/Common/JPLMCodecFactory.h"


int main(int argc, char const* argv[]) {
  auto configuration = JPLMConfigurationFactory::get_configuration<JPLMEncoderConfiguration>(argc, argv);
  auto decoder = JPLMCodecFactory::get_decoder(std::move(configuration));
  decoder->run();
  exit(EXIT_SUCCESS);
}