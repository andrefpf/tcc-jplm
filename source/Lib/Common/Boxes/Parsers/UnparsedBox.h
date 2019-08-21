#ifndef UNPARSEDBOX_H__
#define UNPARSEDBOX_H__

#include <cstdint>

class UnparsedBox
{
protected:
	const uint64_t initial_pos;
	const uint64_t final_pos;
public:
	UnparsedBox(const uint64_t initial, const uint64_t final) : initial_pos(initial), final_pos(final){}
	~UnparsedBox() = default;
	
	auto get_initial_pos() const {
		return initial_pos;
	}

	auto get_final_pos() const {
		return final_pos;
	}
};

#endif /* end of include guard: UNPARSEDBOX_H__ */