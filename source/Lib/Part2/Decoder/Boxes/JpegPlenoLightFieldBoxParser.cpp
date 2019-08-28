/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 16:08:40
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 16:56:17
*/

#include "JpegPlenoLightFieldBoxParser.h"


std::unique_ptr<Box> JPLMBoxParser::JpegPlenoLightFieldBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto& box_parser = BoxParserRegistry::get_instance();

  auto profile_and_level_box = box_parser.parse<ProfileAndLevelBox>(
      box_parser_helper.get_remaining_stream());

  auto jpeg_pleno_light_field_header_box =
      box_parser.parse<JpegPlenoLightFieldHeaderBox>(
          box_parser_helper.get_remaining_stream());

  //! \todo implement contiguous codestream parsing... only reading as a blob? or already decoding??
  // auto contigous_codestream_box =
  //     box_parser_helper.has_data_available()
  //         ? box_parser.parse<ContiguousCodestreamBox, false>(
  //               box_parser_helper.get_remaining_stream())
  //         : nullptr;
  

  //! \todo decode remaining file
  // how? creating a map for the boxes if they have no specific order in the file..
  box_parser_helper.get_remaining_stream().forward();

  auto jpeg_pleno_light_field_contents =
      std::make_unique<JpegPlenoLightFieldContents>(
          std::move(profile_and_level_box), std::move(jpeg_pleno_light_field_header_box));

  auto jpeg_pleno_light_field_box = std::make_unique<JpegPlenoLightFieldBox>(std::move(jpeg_pleno_light_field_contents));
  return jpeg_pleno_light_field_box;
}