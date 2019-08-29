/*
* @Author: Ismael Seidel
* @Date:   2019-08-29 10:42:51
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-29 16:50:46
*/

#include "ContiguousCodestreamBoxParser.h"


uint64_t JPLMBoxParser::ContiguousCodestreamBoxParser::memory_limit = 1024;

std::unique_ptr<Box> JPLMBoxParser::ContiguousCodestreamBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto data_lenght = box_parser_helper.get_data_lenght();

  if (data_lenght <= memory_limit) {
    auto remaining_stream = box_parser_helper.get_remaining_stream();
    assert(remaining_stream.get_length() == data_lenght);
    auto contiguous_codestream_code =
        std::make_unique<ContiguousCodestreamCodeInMemory>(
            remaining_stream.get_n_bytes(data_lenght));
    auto contiguous_codestream_box = std::make_unique<ContiguousCodestreamBox>(
        std::move(contiguous_codestream_code));
    return contiguous_codestream_box;
  }  //else

  // \todo read in a code as pointer to file, or copy to a temporary file...
  //return ;

  return nullptr;
}