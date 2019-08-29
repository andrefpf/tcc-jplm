/*
* @Author: Ismael Seidel
* @Date:   2019-08-29 10:42:51
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-29 11:13:26
*/

#include "ContiguousCodestreamBoxParser.h"

uint64_t JPLMBoxParser::ContiguousCodestreamBoxParser::memory_limit = 1024;

std::unique_ptr<Box> JPLMBoxParser::ContiguousCodestreamBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {

  if(box_parser_helper.get_data_lenght() <= memory_limit ) {
	  while(box_parser_helper.has_data_available()) {
	  	// \todo read in a code as a byte stream
	  }
	  //return ;
  } else {
  	// \todo read in a code as pointer to file, or copy to a temporary file...
  	//return ;
  }

  return nullptr;
}