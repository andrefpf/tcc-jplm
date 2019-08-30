#ifndef BOXPARSER_H__
#define BOXPARSER_H__

#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include "Lib/Common/Boxes/Box.h"
#include "Lib/Common/Boxes/Parsers/ColourSpecificationBoxParser.h"
#include "Lib/Common/Boxes/Parsers/ContiguousCodestreamBoxParser.h"
#include "Lib/Part1/Decoder/Boxes/FileTypeBoxParser.h"
#include "Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldHeaderBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/LightFieldHeaderBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/ProfileAndLevelBoxParser.h"
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


  ParsedBox parse(ManagedStream&& managed_stream) const;


  template<class ParsingBox, bool required = true>
  std::unique_ptr<ParsingBox> parse(ManagedStream&& managed_stream) const {
    // std::cout << "ManagedStream is at: " << managed_stream.tell() << std::endl;
    // std::cout << "ManagedStream is limited to: " << managed_stream.get_length() << std::endl;
    // std::cout << typeid(ParsingBox).name() << std::endl;
    auto box_parser_helper =
        BoxParserHelper<ParsingBox, required>(managed_stream);
    // std::cout << "created helper here <>" << std::endl;
    if constexpr (!required) {
      if (!box_parser_helper.is_a_box_with_id(ParsingBox::id)) {
        //this means that the current box being read is not of the type ParsingBox
        //therefore, it can rewind the stream to allow for a next box to read again the l and t boxes
        managed_stream.rewind();
        return nullptr;
      }
    }
    auto parsed_box = parse(box_parser_helper);
    if (parsed_box) {  //i.e., the unique_ptr is not null
      return std::unique_ptr<ParsingBox>(
          static_cast<ParsingBox*>(parsed_box.release()));
    }
    return nullptr;
  }


  ParsedBox parse(BoxParserHelperBase& box_parser_helper) const;


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