#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/JPLMCodecFactory.h"
#include "Lib/Common/JPLMConfigurationFactory.h"
#include "Lib/Part1/Decoder/JPLFileFromStream.h"





int main(int argc, char const* argv[]) {
  auto configuration =
      JPLMConfigurationFactory::get_decoder_configuration(argc, argv);

  std::cout << configuration->get_input_filename() << std::endl;
  std::cout << configuration->get_output_filename() << std::endl;

  auto jpl_file =
      std::make_shared<JPLFileFromStream>(configuration->get_input_filename());

  auto decoders = JPLMCodecFactory::get_decoders(jpl_file, configuration->get_output_filename());
  for(auto& decoder : decoders) {
    decoder->run();
  }

  exit(EXIT_SUCCESS);
}