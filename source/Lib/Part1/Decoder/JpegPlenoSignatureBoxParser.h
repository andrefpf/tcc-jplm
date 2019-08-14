#ifndef JPEGPLENOSIGNATUREBOXPARSER_H__
#define JPEGPLENOSIGNATUREBOXPARSER_H__

#include <memory>
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "Lib/Utils/Stream/ManagedStream.h"

namespace JPLMBoxParser {
class JpegPlenoSignatureBoxParser {
 public:
 	static uint32_t id;
  static std::optional<std::unique_ptr<Box>> parse(ManagedStream& managed_stream);
};
}  // namespace JPLMBoxParser

#endif /* end of include guard: JPEGPLENOSIGNATUREBOXPARSER_H__ */