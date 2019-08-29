#ifndef CONTIGUOUSCODESTREAMBOXPARSER_H__
#define CONTIGUOUSCODESTREAMBOXPARSER_H__

#include <memory>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamBox.h"
#include "Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"

namespace JPLMBoxParser {
class ContiguousCodestreamBoxParser {
 public:
  using ParsingBox = ContiguousCodestreamBox;
  static uint64_t memory_limit;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser


//
// ContiguousCodestreamBoxParser::memory_limit = value_found_in_config;

#endif /* end of include guard: CONTIGUOUSCODESTREAMBOXPARSER_H__ */