/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 17:01:09
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-12 17:46:00
*/

#include "ManagedStream.h"


ManagedStream::ManagedStream(
    std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos)
    : ref_to_stream(ref_to_stream), initial_pos(initial_pos),
      final_pos(final_pos) {
   	auto offset = static_cast<int64_t>(initial_pos);
    if(ref_to_stream.tellg() != offset) {
    	if(offset < 0) {
    		throw ManagedStreamExceptions::OverflowOfInitialPositionException(initial_pos);
    	}
    	//this guarantees that the stream is at the begining
    	ref_to_stream.seekg(offset, std::ios_base::beg);
    }
}


ManagedStream::ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset)
    : ManagedStream(ref_to_stream, ref_to_stream.tellg(),
          static_cast<uint64_t>(ref_to_stream.tellg()) + max_offset) {
}


uint64_t ManagedStream::get_current_pos() const noexcept {
	return ref_to_stream.tellg();
}