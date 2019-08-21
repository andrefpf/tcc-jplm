/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:08:50
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-16 19:23:00
*/

#include "JpegPlenoSignatureBoxParser.h"

std::unique_ptr<Box> JPLMBoxParser::JpegPlenoSignatureBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {  //managed_stream

  auto managed_stream = box_parser_helper.get_data_stream();

  auto bytes = managed_stream.get_bytes<4>();

  auto jpeg_pleno_signature_box = std::make_unique<JpegPlenoSignatureBox>();
  if (!jpeg_pleno_signature_box->is_valid(bytes)) {
    throw JpegPlenoSignatureBoxParserExceptions::InvalidJpegPlenoSignatureBox();
  }
  return jpeg_pleno_signature_box;
}
