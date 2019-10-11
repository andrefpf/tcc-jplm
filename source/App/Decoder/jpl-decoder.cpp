#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "Lib/Common/JPLMCodecFactory.h"
#include "Lib/Common/JPLMConfigurationFactory.h"
#include "Lib/Part1/Decoder/JPLFileFromStream.h"
#include "Lib/Part2/Decoder/TransformMode/JPLM4DTransformModeLightFieldDecoder.h"


int main([[maybe_unused]] int argc, [[maybe_unused]] char const* argv[]) {
  // auto configuration = JPLMConfigurationFactory::get_encoder_configuration(argc, argv);
  auto configuration =
      JPLMConfigurationFactory::get_decoder_configuration(argc, argv);

  std::cout << configuration->get_input_filename() << std::endl;
  std::cout << configuration->get_output_filename() << std::endl;

  auto jpl_file =
      std::make_unique<JPLFileFromStream>(configuration->get_input_filename());
  std::cout << "jpl" << std::endl;
  if (!jpl_file->has_codestream()) {
    std::cerr << "The input jpl file has no codestream" << std::endl;
    exit(1);
  }
  // auto& codestreams = jpl_file->get_reference_to_codestreams();
  // for (const auto& codestream : codestreams) {
  //   switch (codestream->get_type()) {
  //     case JpegPlenoCodestreamBoxTypes::LightField: {
  //     	std::cout << "is a LightField" << std::endl;
  //     	// const auto& codestream_as_part2 = static_cast<JpegPlenoLightFieldBox&>(*codestream);
  //     	std::cout << "a" << std::endl;
  //     	// const auto& contiguous = codestream_as_part2.get_ref_to_contents().get_ref_to_contiguous_codestream_box();
  //     	std::cout << "b" << std::endl;
  //     	// auto decoder = JPLM4DTransformModeLightFieldDecoder<uint16_t>(*jpl_file, codestream_as_part2, configuration->get_output_filename());
  //     	// decoder.run();
  //       break;
  //     }
  //     case JpegPlenoCodestreamBoxTypes::PointCloud: {
  //     	std::cout  << "point cloud" << std::endl;
  //     	std::cerr << "Not defined yet..." << std::endl;
  //       break;
  //     }
  //     case JpegPlenoCodestreamBoxTypes::Hologram: {
  //     	std::cout  << "hologram" << std::endl;
  //     	std::cerr << "Not defined yet..." << std::endl;
  //       break;
  //     }
  //   }
  // }
  // JPLFileFromStream
  // auto decoder = JPLMCodecFactory::get_decoder(std::move(configuration));
  // decoder->run();
  exit(EXIT_SUCCESS);
}