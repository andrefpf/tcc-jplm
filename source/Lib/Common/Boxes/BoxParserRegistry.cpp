/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 15:51:14
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-16 11:20:46
*/

#include "BoxParserRegistry.h"


BoxParserRegistry& BoxParserRegistry::get_instance() {
  static BoxParserRegistry instance;
  return instance;
}


std::map<uint32_t, BoxParserRegistry::ParsingFunction>&
BoxParserRegistry::get_ref_to_parser_map() {
  static std::map<uint32_t, BoxParserRegistry::ParsingFunction> parser_map;
  return parser_map;
}


// auto box_parser_helper = BoxParserHelper<ParsingBox>(managed_stream);
BoxParserRegistry::ParsedBox BoxParserRegistry::parse(
    ManagedStream& managed_stream) {
  auto box_parser_helper = BoxParserHelperBase(managed_stream);
  return parse(box_parser_helper);
}


BoxParserRegistry::ParsedBox BoxParserRegistry::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto& map = BoxParserRegistry::get_ref_to_parser_map();
  if (auto it = map.find(box_parser_helper.get_t_box_value());
      it != map.end()) {
    std::cout << "found parsing method for id 0x" << std::hex
              << std::setfill('0') << std::setw(8)
              << box_parser_helper.get_t_box_value() << std::dec << std::endl;
    return it->second(box_parser_helper);
  }
  std::cout << "not found " << box_parser_helper.get_t_box_value() << std::endl;
  return nullptr;
}


void BoxParserRegistry::register_known_parsers() {
  //add here known parsers
  using namespace JPLMBoxParser;
  register_parser<JpegPlenoSignatureBoxParser>();
}
