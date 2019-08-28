/*
* @Author: Ismael Seidel
* @Date:   2019-08-28 13:04:04
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-28 14:00:19
*/

#include "JpegPlenoLightFieldHeaderBoxParser.h"


std::unique_ptr<Box> JPLMBoxParser::JpegPlenoLightFieldHeaderBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  // auto light_field_header_box =
  //     box_parser_helper.get_next_box<LightFieldHeaderBox>();
  // if (!light_field_header_box) {
  //   //! \todo throw (light field header box is not optional)
  // }

  // auto bits_per_component_box =
  //     box_parser_helper
  //         .get_next_box<BitsPerComponentBox>();  //optional, may be nullptr

  // std::vector<std::unique_ptr<ColourSpecificationBox>> colr;

  // while (true) {
  //   auto colour_specification_box =
  //       box_parser_helper.get_next_box<ColourSpecificationBox>();
  //   if (colour_specification_box) {
  //     colr.emplace_back(std::move(colour_specification_box));
  //   } else {
  //     break;
  //   }
  // }

  // if (colr.size() == 0) {
  //   //! \todo throw (at least one colour specification box must be present)
  // }

  // auto channel_definition_box =
  //     box_parser_helper.get_next_box<ChannelDefinitionBox>();

  // auto light_field_header_contents =
  //     std::make_unique<JpegPlenoLightFieldHeaderContents>(
  //         std::move(light_field_header_box), std::move(bits_per_component_box),
  //         std::move(colr), std::move(channel_definition_box));

  // auto light_field_header_box = std::make_unique<JpegPlenoLightFieldHeaderBox>(
  //     std::move(light_field_header_contents));
  // return light_field_header_box;
  return nullptr;
}