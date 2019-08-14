/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 15:51:14
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-14 18:11:18
*/

#include "BoxParser.h"



BoxParser& BoxParser::get_instance() {
	static BoxParser instance;
	return instance;
}


std::map<uint32_t, std::function<BoxParser::ParsedBox(ManagedStream&)>>&
BoxParser::get_ref_to_parser_map() {
  static std::map<uint32_t, std::function<ParsedBox(ManagedStream&)>>
      parser_map;
  return parser_map;
}


BoxParser::ParsedBox BoxParser::parse(uint32_t t_box_code) {
  auto& map = BoxParser::get_ref_to_parser_map();
  std::cout << "Tried to parse a box " << t_box_code << std::endl;
  if (auto it = map.find(t_box_code); it != map.end()) {
    std::cout << "found " << t_box_code << std::endl;
    return nullptr;  //std::move(it->second(managed_stream));
  }
  std::cout << "not found " << t_box_code << std::endl;
  return nullptr;
}


void BoxParser::register_known_parsers() {
	//add here known parsers
  using namespace JPLMBoxParser;
  // BoxParser::get_ref_to_parser_map()
  register_parser<JpegPlenoSignatureBoxParser>();
}
