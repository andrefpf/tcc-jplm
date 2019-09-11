#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Part2/Encoder/JPLM4DTransformModeLightFieldEncoder.h"


int main(int argc, char const* argv[]) {
  auto configuration = ConfigurationFactory::get_configuration(argc, argv);
  std::ofstream of_stream(
      configuration->get_output_filename(), std::ofstream::binary);
  if (!of_stream.is_open()) {
    std::cerr << "Error opening output file" << std::endl;
    exit(EXIT_FAILURE);
  }
  auto encoder = CodecFactory::get_encoder(std::move(configuration));
  encoder.run();
  const auto& jpl_file = encoder.get_ref_to_jpl_file();
  of_stream << jpl_file;

  exit(EXIT_SUCCESS);
}