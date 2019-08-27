/*
* @Author: Ismael Seidel
* @Date:   2019-08-27 18:41:10
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-27 18:52:28
*/

#include "LightFieldHeaderBoxParser.h"

std::unique_ptr<Box> JPLMBoxParser::LightFieldHeaderBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  const auto t = box_parser_helper.get_next<uint32_t>();
  const auto s = box_parser_helper.get_next<uint32_t>();
  const auto v = box_parser_helper.get_next<uint32_t>();
  const auto u = box_parser_helper.get_next<uint32_t>();

  const auto nc = box_parser_helper.get_next<uint16_t>();
  const auto bpc = box_parser_helper.get_next<uint8_t>();
  const auto c = box_parser_helper.get_next<compression_type_data>();
  const auto unkc =
      box_parser_helper.get_next<colour_space_unknown_flag_type>();
  const auto ipr = box_parser_helper.get_next<intelectual_property_flag_type>();

  auto light_field_header_contents = LightFieldHeaderContents({t, s, v, u}, nc,
      bpc, CompressionTypeLightField{c}, ColourSpaceUnknownFlag{unkc},
      IntelectualPropertyFlag{ipr});

  auto light_field_header_box =
      std::make_unique<LightFieldHeaderBox>(std::move(light_field_header_contents));
  return light_field_header_box;
}