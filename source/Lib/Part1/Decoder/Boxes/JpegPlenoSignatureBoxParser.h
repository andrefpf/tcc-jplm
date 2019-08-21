#ifndef JPEGPLENOSIGNATUREBOXPARSER_H__
#define JPEGPLENOSIGNATUREBOXPARSER_H__

#include <memory>
#include "source/Lib/Common/Boxes/Parsers/BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "source/Lib/Part1/Common/Boxes/JpegPlenoSignatureBox.h"

namespace JPLMBoxParser {
class JpegPlenoSignatureBoxParser {
 public:
  using ParsingBox = JpegPlenoSignatureBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: JPEGPLENOSIGNATUREBOXPARSER_H__ */