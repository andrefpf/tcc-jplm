#ifndef BOXPARSER_H__
#define BOXPARSER_H__

#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include "Lib/Part1/Common/Box.h"
#include "Lib/Utils/Stream/ManagedStream.h"
#include "Lib/Part1/Decoder/JpegPlenoSignatureBoxParser.h"

class BoxParser
{
	using ParsedBox = std::optional<std::unique_ptr<Box>>;
private:
	void register_known_parsers();
	BoxParser() {register_known_parsers();}
	~BoxParser() = default;
public:

	static BoxParser& get_instance();

	
	static std::map<uint32_t, std::function<ParsedBox(ManagedStream&)>>& get_ref_to_parser_map();

	
	ParsedBox parse(uint32_t t_box_code, ManagedStream& managed_stream);

	
	template<class ParserClass> 
	void register_parser() {
		auto& map = BoxParser::get_ref_to_parser_map();
		const auto id = ParserClass::id;
		auto parsing_function = ParserClass::parse;
		std::cout << "Registered " << id << std::endl;
		map[id] = parsing_function;
	}
	


};

#endif /* end of include guard: BOXPARSER_H__ */