#ifndef LIGHTFIELDHEADERBOXPARSER_H__
#define LIGHTFIELDHEADERBOXPARSER_H__



#include <memory>
#include "source/Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "source/Lib/Part2/Common/Boxes/LightFieldHeaderBox.h"

namespace JPLMBoxParser {
class LightFieldHeaderBoxParser {
 public:
  using ParsingBox = LightFieldHeaderBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: LIGHTFIELDHEADERBOXPARSER_H__ */