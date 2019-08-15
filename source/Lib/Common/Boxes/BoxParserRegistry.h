#ifndef BOXPARSER_H__
#define BOXPARSER_H__

#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include "Lib/Part1/Common/Box.h"
#include "Lib/Part1/Decoder/JpegPlenoSignatureBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxParserRegistry {
  using ParsedBox = std::optional<std::unique_ptr<Box>>;

 private:
  void register_known_parsers();
  BoxParserRegistry() {
    register_known_parsers();
  }
  ~BoxParserRegistry() = default;

 public:
  static BoxParserRegistry& get_instance();


  static std::map<uint32_t, std::function<ParsedBox(ManagedStream&)>>&
  get_ref_to_parser_map();


  ParsedBox parse(uint32_t t_box_code, ManagedStream& managed_stream);


  template<class ParserClass>
  void register_parser() {
    auto& map = BoxParserRegistry::get_ref_to_parser_map();
    const auto id = ParserClass::ParsingBox::id;
    auto parsing_function = ParserClass::parse;
    std::cout << "Registered 0x" << std::hex << std::setfill('0') << std::setw(8)
              << id << std::endl;
    map[id] = parsing_function;
  }
};

#endif /* end of include guard: BOXPARSER_H__ */