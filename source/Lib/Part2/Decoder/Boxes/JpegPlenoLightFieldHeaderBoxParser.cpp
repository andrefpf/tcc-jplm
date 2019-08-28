/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 13:04:04
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 14:47:38
*/

#include "JpegPlenoLightFieldHeaderBoxParser.h"


std::unique_ptr<Box> JPLMBoxParser::JpegPlenoLightFieldHeaderBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto& box_parser = BoxParserRegistry::get_instance();

  auto light_field_header_box = box_parser.parse<LightFieldHeaderBox>(
      box_parser_helper.get_remaining_stream());

  auto bits_per_component_box = box_parser.parse<BitsPerComponentBox, false>(
      box_parser_helper.get_remaining_stream());  //optional, may be nullptr

  std::vector<std::unique_ptr<ColourSpecificationBox>> colr;

  while (true) {
    auto colour_specification_box =
        box_parser.parse<ColourSpecificationBox, false>(
            box_parser_helper.get_remaining_stream());
    if (colour_specification_box) {
      colr.emplace_back(std::move(colour_specification_box));
    } else {
      break;
    }
  }

  if (colr.size() == 0) {
    //! \todo throw (at least one colour specification box must be present)
  }

  auto channel_definition_box = box_parser.parse<ChannelDefinitionBox, false>(
      box_parser_helper.get_remaining_stream());

  auto light_field_header_contents =
      std::make_unique<JpegPlenoLightFieldHeaderContents>(
          std::move(light_field_header_box), std::move(bits_per_component_box),
          std::move(colr), std::move(channel_definition_box));

  auto jpeg_pleno_light_field_header_box = std::make_unique<JpegPlenoLightFieldHeaderBox>(
      std::move(light_field_header_contents));
  return jpeg_pleno_light_field_header_box;
}