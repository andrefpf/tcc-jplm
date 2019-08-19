#ifndef JPEGPLENOSIGNATUREBOXPARSER_H__
#define JPEGPLENOSIGNATUREBOXPARSER_H__

#include <memory>
#include "BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "Lib/Part1/Common/JpegPlenoSignatureBox.h"

namespace JPLMBoxParser {
class JpegPlenoSignatureBoxParser {
 public:
  using ParsingBox = JpegPlenoSignatureBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: JPEGPLENOSIGNATUREBOXPARSER_H__ */