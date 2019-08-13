/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 17:01:09
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-13 14:06:46
*/

#include "ManagedStream.h"


ManagedStream::ManagedStream(
    std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos)
    : ref_to_stream(ref_to_stream), initial_pos(initial_pos),
      final_pos(final_pos) {
  if (!ref_to_stream.is_open()) {
    throw ManagedStreamExceptions::ClosedStreamException();
  }
  if (initial_pos == final_pos) {
    throw ManagedStreamExceptions::
        InvalidEqualInitialAndFinalPositionException();
  }
  auto offset = static_cast<int64_t>(initial_pos);
  if (ref_to_stream.tellg() != offset) {
    if (offset < 0) {
      throw ManagedStreamExceptions::OverflowOfInitialPositionException(
          initial_pos);
    }
    //this guarantees that the stream is at the begining
    ref_to_stream.seekg(offset, std::ios_base::beg);
  }
}


ManagedStream::ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset)
    : ManagedStream(ref_to_stream, ref_to_stream.tellg(),
          static_cast<uint64_t>(ref_to_stream.tellg()) + max_offset) {
}


bool ManagedStream::index_is_valid(uint64_t index) const noexcept {
  if ((index < initial_pos) || (index >= final_pos)) {
    return false;
  }
  return true;
}


//this sets the stream to the begining of the sub managed stream
ManagedStream ManagedStream::get_sub_managed_stream(
    uint64_t initial_pos, uint64_t final_pos) {
  if (!index_is_valid(initial_pos) || (final_pos > this->final_pos)) {
    throw ManagedStreamExceptions::InvalidIndexForSubManagedStreamException(
        initial_pos, final_pos, this->initial_pos, this->final_pos);
  }
  return {ref_to_stream, initial_pos, final_pos};
}


ManagedStream ManagedStream::get_sub_managed_stream(uint64_t max_offset) {
  return get_sub_managed_stream(ref_to_stream.tellg(),
      static_cast<uint64_t>(ref_to_stream.tellg()) + max_offset);
}


ManagedStream& ManagedStream::rewind() {
  ref_to_stream.seekg(static_cast<int64_t>(initial_pos), std::ios_base::beg);
  return *this;
}


ManagedStream& ManagedStream::forward() {
  ref_to_stream.seekg(static_cast<int64_t>(final_pos), std::ios_base::beg);
  ref_to_stream.peek();
  if (ref_to_stream.eof()) {
    throw ManagedStreamExceptions::TryingToAccessBeyondEOFException();
  }
  return *this;
}


ManagedStream& ManagedStream::seek(
    int64_t offset, const std::ios_base::seekdir relative_to) {
  switch (relative_to) {
    case std::ios_base::beg: {
      ref_to_stream.seekg(initial_pos + offset, std::ios_base::beg);
      break;
    }
    case std::ios_base::cur: {
      ref_to_stream.seekg(offset, std::ios_base::cur);
      break;
    }
    default:
      throw ManagedStreamExceptions::UnknownSeekDirectionException();
  }
  return *this;
}


uint64_t ManagedStream::get_current_pos() const noexcept {
  return ref_to_stream.tellg();
}