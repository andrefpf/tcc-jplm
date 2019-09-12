/*
* @Author: Ismael Seidel
* @Date:   2019-08-12 17:01:09
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-08-30 10:44:41
*/

#include "ManagedStream.h"


ManagedStream::ManagedStream(
    std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos)
    : ref_to_stream(ref_to_stream), initial_pos(initial_pos),
      final_pos(final_pos) {
  if (!ref_to_stream.is_open()) {
    throw ManagedStreamExceptions::ClosedStreamException();
  }
  if (initial_pos >= final_pos) {
    if (initial_pos == final_pos) {
      throw ManagedStreamExceptions::
          InvalidEqualInitialAndFinalPositionException();
    }
    throw ManagedStreamExceptions::FinalPositioSmallerThanInitialException(
        initial_pos, final_pos);
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
  // std::cout << "created managed stream [" << initial_pos << ", " << final_pos << "[" << std::endl;
}


ManagedStream::ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset)
    : ManagedStream(ref_to_stream, ref_to_stream.tellg(),
          static_cast<uint64_t>(ref_to_stream.tellg()) + max_offset) {
}


bool ManagedStream::is_valid(uint64_t index) const noexcept {
  if ((index < initial_pos) || (index >= final_pos)) {
    return false;
  }
  return true;
}


//in this context, is_valid means that there is more bytes to be readed
bool ManagedStream::is_valid() const noexcept {
  return is_valid(tell());
}


//this sets the stream to the begining of the sub managed stream
ManagedStream ManagedStream::get_sub_managed_stream(
    uint64_t initial_pos, uint64_t final_pos) {
  if (!is_valid(initial_pos) || (final_pos > this->final_pos)) {
    throw ManagedStreamExceptions::InvalidIndexForSubManagedStreamException(
        initial_pos, final_pos, this->initial_pos, this->final_pos);
  }
  return {ref_to_stream, initial_pos, final_pos};
}


// gets a managed stream starding at the current pos with offset max_offset
ManagedStream ManagedStream::get_sub_managed_stream(uint64_t max_offset) {
  return get_sub_managed_stream(ref_to_stream.tellg(),
      static_cast<uint64_t>(ref_to_stream.tellg()) + max_offset);
}


ManagedStream ManagedStream::get_remaining_sub_managed_stream() {
  // return get_sub_managed_stream(this->get_length() - (initial_pos+this->tell()));
  return get_sub_managed_stream(ref_to_stream.tellg(), this->final_pos);
}


ManagedStream& ManagedStream::rewind() {
  ref_to_stream.seekg(static_cast<int64_t>(initial_pos), std::ios_base::beg);
  return *this;
}

//! \todo check if it is necessary to perform some check in the eof (perhaps when getting data...)
// ref_to_stream.peek();
// if (ref_to_stream.eof()) {
//   throw ManagedStreamExceptions::TryingToAccessBeyondEOFException();
// }
// 

ManagedStream& ManagedStream::forward() {
  ref_to_stream.seekg(static_cast<int64_t>(final_pos), std::ios_base::beg);
  return *this;
}


ManagedStream& ManagedStream::seek(
    int64_t offset, const std::ios_base::seekdir relative_to) {
  switch (relative_to) {
    case std::ios_base::beg: {
      if (offset < 0) {
        throw ManagedStreamExceptions::SeekBeforeInitialPositionException();
      }
      auto seek_to_pos = initial_pos + offset;
      if (seek_to_pos > final_pos) {
        throw ManagedStreamExceptions::SeekAfterFinalPositionException();
      }
      ref_to_stream.seekg(seek_to_pos, std::ios_base::beg);
      break;
    }
    case std::ios_base::cur: {
      auto current_position = static_cast<int64_t>(ref_to_stream.tellg());
      if (current_position + offset < static_cast<int64_t>(initial_pos)) {
        throw ManagedStreamExceptions::SeekBeforeInitialPositionException();
      }
      if (current_position + offset > static_cast<int64_t>(final_pos)) {
        throw ManagedStreamExceptions::SeekAfterFinalPositionException();
      }
      ref_to_stream.seekg(offset, std::ios_base::cur);
      break;
    }
    default:
      throw ManagedStreamExceptions::UnknownSeekDirectionException();
  }
  return *this;
}


uint64_t ManagedStream::tell() const noexcept {
  return get_current_pos();
}


uint64_t ManagedStream::get_current_pos() const noexcept {
  return ref_to_stream.tellg();
}

uint64_t ManagedStream::get_length() const noexcept {
  return final_pos - initial_pos;
}