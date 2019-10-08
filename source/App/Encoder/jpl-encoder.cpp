#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/JPLMConfigurationFactory.h"
#include "Lib/Common/JPLMCodecFactory.h"


int main([[maybe_unused]]int argc, [[maybe_unused]]char const* argv[]) {
    auto configuration = JPLMConfigurationFactory::get_encoder_configuration(argc, argv);
//  std::ofstream of_stream(
//      configuration->get_output_filename(), std::ofstream::binary);
//  if (!of_stream.is_open()) {
//    std::cerr << "Error opening output file" << std::endl;
//    exit(EXIT_FAILURE);
//  }
//  auto encoder = JPLMCodecFactory::get_encoder(std::move(configuration));
//  encoder->run();
//  const auto& jpl_file = encoder->get_ref_to_jpl_file();
//  of_stream << jpl_file;

  exit(EXIT_SUCCESS);
}