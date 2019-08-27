#ifndef PROFILEANDLEVELBOXPARSER_H__
#define PROFILEANDLEVELBOXPARSER_H__


#include <memory>
#include "source/Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "source/Lib/Part2/Common/Boxes/ProfileAndLevelBox.h"

namespace JPLMBoxParser {
class ProfileAndLevelBoxParser {
 public:
  using ParsingBox = ProfileAndLevelBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser


#endif /* end of include guard: PROFILEANDLEVELBOXPARSER_H__ */