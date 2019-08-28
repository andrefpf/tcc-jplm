#ifndef JPEGPLENOLIGHTFIELDBOXPARSER_H__
#define JPEGPLENOLIGHTFIELDBOXPARSER_H__


#include <memory>
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "source/Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "source/Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "source/Lib/Part2/Common/Boxes/JpegPlenoLightFieldBox.h"

namespace JPLMBoxParser {
class JpegPlenoLightFieldBoxParser {
 public:
  using ParsingBox = JpegPlenoLightFieldBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: JPEGPLENOLIGHTFIELDBOXPARSER_H__ */