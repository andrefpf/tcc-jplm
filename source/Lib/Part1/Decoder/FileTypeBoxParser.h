#ifndef FILETYPEBOXPARSER_H__
#define FILETYPEBOXPARSER_H__

#include <memory>
#include "BoxParserHelper.h"
#include "Lib/Part1/Common/DefinedBoxes.h"
#include "Lib/Part1/Common/FileTypeBox.h"

namespace JPLMBoxParser {
class FileTypeBoxParser {
 public:
  using ParsingBox = FileTypeBox;
  static std::unique_ptr<Box> parse(
      BoxParserHelperBase& box_parser_helper);  //box parser helper
};
}  // namespace JPLMBoxParser


#endif /* end of include guard: FILETYPEBOXPARSER_H__ */