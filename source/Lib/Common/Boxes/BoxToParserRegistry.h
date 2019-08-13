#ifndef BOXTOPARSERREGISTRY_H__
#define BOXTOPARSERREGISTRY_H__

#include <functional>
#include <memory>
#include <map>
#include "Lib/Part1/Common/Box.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxToParserRegistry
{
	// typedef std::function<Box*(ManagedStream&)> ParserSignature;
	typedef std::function<void()> ParserSignature;
	static std::map<uint32_t, ParserSignature> map;

public:
	BoxToParserRegistry() = default;


	~BoxToParserRegistry() = default;
	

	static bool register_box(const uint32_t t_box_code, ParserSignature parser_function) {
		if(auto it = map.find(t_box_code); it == map.end()) { //not registered yet
			map[t_box_code] = parser_function;
			return true;
		}
		return false;
	}


	static Box* parse_box(uint32_t t_box_code, ManagedStream& ) { //managed_stream

		if(auto it = map.find(t_box_code); it != map.end()) {
			return nullptr;//std::move(it->second(managed_stream));
		}
		return nullptr;
	}


	bool is_known(const TBox& t_box) const noexcept;
};

#endif /* end of include guard: BOXTOPARSERREGISTRY_H__ */