#ifndef BOXPARSER_H__
#define BOXPARSER_H__

#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include "source/Lib/Common/Boxes/Box.h"
#include "source/Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h"
#include "source/Lib/Part1/Decoder/Boxes/FileTypeBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxParserRegistry {
  using ParsedBox = std::unique_ptr<Box>;
  using ParsingFunction = std::function<ParsedBox(BoxParserHelperBase&)>;

 private:
  void register_known_parsers();

  
  BoxParserRegistry() {
    register_known_parsers();
  }


  ~BoxParserRegistry() = default;

 public:
  static BoxParserRegistry& get_instance();


  static std::map<uint32_t, ParsingFunction>& get_ref_to_parser_map();


  ParsedBox parse(ManagedStream& managed_stream) const;


  template<class ParsingBox>
  std::unique_ptr<ParsingBox> parse(ManagedStream& managed_stream) const {
  	// std::cout << "ManagedStream is at: " << managed_stream.tell() << std::endl;
  	// std::cout << typeid(ParsingBox).name() << std::endl;
    auto box_parser_helper = BoxParserHelper<ParsingBox>(managed_stream);
    // std::cout << "here!!!";
    auto parsed_box = parse(box_parser_helper);
    if (parsed_box) { //i.e., the unique_ptr is not null
    	// std::cout << "not null\n";
    	return std::unique_ptr<ParsingBox>(static_cast<ParsingBox*>(parsed_box.release()));
    }
    return nullptr;
  }


  ParsedBox parse(BoxParserHelperBase& box_parser_helper) const ;


  template<class ParserClass>
  void register_parser() {
    auto& map = BoxParserRegistry::get_ref_to_parser_map();
    const auto id = ParserClass::ParsingBox::id;
    auto parsing_function = ParserClass::parse;
    // std::cout << "Registered 0x" << std::hex << std::setfill('0')
    //           << std::setw(8) << id << std::endl;
    map[id] = parsing_function;
  }
};

#endif /* end of include guard: BOXPARSER_H__ */