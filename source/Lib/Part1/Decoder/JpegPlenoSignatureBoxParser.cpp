/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:08:50
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-14 18:19:12
*/

#include "JpegPlenoSignatureBoxParser.h"

uint32_t JPLMBoxParser::JpegPlenoSignatureBoxParser::id = 0;

std::optional<std::unique_ptr<Box>> JPLMBoxParser::JpegPlenoSignatureBoxParser::parse(ManagedStream& ) { //managed_stream
	std::cout << "Parsing JpegPlenoSignatureBoxParser" << std::endl;
	return nullptr;
}

