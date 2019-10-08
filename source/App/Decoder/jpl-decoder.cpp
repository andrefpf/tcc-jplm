#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/JPLMConfigurationFactory.h"
#include "Lib/Common/JPLMCodecFactory.h"


int main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) {
  auto configuration = JPLMConfigurationFactory::get_encoder_configuration(argc, argv);
  auto decoder = JPLMCodecFactory::get_decoder(std::move(configuration));
  decoder->run();
  exit(EXIT_SUCCESS);
}