/*
* @Author: Ismael Seidel
* @Date:   2019-08-14 13:08:50
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-15 17:24:57
*/

#include "JpegPlenoSignatureBoxParser.h"

std::optional<std::unique_ptr<Box>> JPLMBoxParser::JpegPlenoSignatureBoxParser::parse(ManagedStream& managed_stream) { //managed_stream
	
	using namespace BinaryTools;
	auto box_parser_helper = BoxParserHelper<ParsingBox>(managed_stream);
	//after the helper is created, the next byte of managed_stream must be from data.
	//box_parser_helper also contains the lenght of the box

	std::cout << "Parsing JpegPlenoSignatureBoxParser" << std::endl;
	std::cout << "The lenght of this box is: " << box_parser_helper.get_length() << std::endl;
	return nullptr;
}

