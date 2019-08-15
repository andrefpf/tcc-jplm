/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 15:51:14
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-15 17:03:23
*/

#include "BoxParserRegistry.h"



BoxParserRegistry& BoxParserRegistry::get_instance() {
	static BoxParserRegistry instance;
	return instance;
}


std::map<uint32_t, std::function<BoxParserRegistry::ParsedBox(ManagedStream&)>>&
BoxParserRegistry::get_ref_to_parser_map() {
  static std::map<uint32_t, std::function<ParsedBox(ManagedStream&)>>
      parser_map;
  return parser_map;
}


BoxParserRegistry::ParsedBox BoxParserRegistry::parse(uint32_t t_box_code, ManagedStream& managed_stream) {
  auto& map = BoxParserRegistry::get_ref_to_parser_map();
  std::cout << "Tried to parse a box " << t_box_code << std::endl;
  if (auto it = map.find(t_box_code); it != map.end()) {
    std::cout << "found parsing method for id 0x" << std::hex << std::setfill('0') << std::setw(8) << t_box_code << std::dec << std::endl;
    return it->second(managed_stream);
  }
  std::cout << "not found " << t_box_code << std::endl;
  return nullptr;
}


void BoxParserRegistry::register_known_parsers() {
	//add here known parsers
  using namespace JPLMBoxParser;
  register_parser<JpegPlenoSignatureBoxParser>();
}
