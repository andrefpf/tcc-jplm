#ifndef COLOURSPECIFICATIONBOXPARSER_H__
#define COLOURSPECIFICATIONBOXPARSER_H__

#include <memory>
#include "Lib/Common/Boxes/Generic/ColourSpecificationBox.h"
#include "Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "Lib/Common/Boxes/Parsers/BoxParserRegistry.h"

namespace JPLMBoxParser {
class ColourSpecificationBoxParser {
 public:
  using ParsingBox = ColourSpecificationBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: COLOURSPECIFICATIONBOXPARSER_H__ */