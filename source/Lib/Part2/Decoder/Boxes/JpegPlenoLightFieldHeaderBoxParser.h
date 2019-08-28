#ifndef JPEGPLENOLIGHTFIELDHEADERBOXPARSER_H__
#define JPEGPLENOLIGHTFIELDHEADERBOXPARSER_H__

#include <memory>
#include "source/Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "source/Lib/Common/Boxes/Parsers/BoxParserRegistry.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "source/Lib/Part2/Common/Boxes/JpegPlenoLightFieldHeaderBox.h"

namespace JPLMBoxParser {
class JpegPlenoLightFieldHeaderBoxParser {
 public:
  using ParsingBox = JpegPlenoLightFieldHeaderBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: JPEGPLENOLIGHTFIELDHEADERBOXPARSER_H__ */