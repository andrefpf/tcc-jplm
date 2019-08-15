#ifndef JPEGPLENOSIGNATUREBOXPARSER_H__
#define JPEGPLENOSIGNATUREBOXPARSER_H__

#include <memory>
#include "BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "Lib/Part1/Common/JpegPlenoSignatureBox.h"
#include "Lib/Utils/Stream/ManagedStream.h"

namespace JPLMBoxParser {
class JpegPlenoSignatureBoxParser {
 public:
  using ParsingBox = JpegPlenoSignatureBox;
  static std::optional<std::unique_ptr<Box>> parse(
      ManagedStream& managed_stream);  //box parser helper
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: JPEGPLENOSIGNATUREBOXPARSER_H__ */