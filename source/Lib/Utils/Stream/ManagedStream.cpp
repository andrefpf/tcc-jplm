/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     ManagedStream.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-12
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


/**
 * \brief      Rewinds the managed stream to its beginning
 *
 * \return     Reference to the current ManagedStream (this)
 */
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


std::size_t ManagedStream::tell() const noexcept {
  return get_current_pos();
}


std::size_t ManagedStream::get_current_pos() const noexcept {
  return ref_to_stream.tellg();
}

std::size_t ManagedStream::get_length() const noexcept {
  return final_pos - initial_pos;
}

std::size_t ManagedStream::get_initial_pos() const noexcept {
  return initial_pos;
}

std::size_t ManagedStream::get_final_pos() const noexcept {
  return final_pos;
}

void ManagedStream::dynamic_assert_access_bounds(const uint64_t n) const {
  auto current_position = static_cast<uint64_t>(ref_to_stream.tellg());
  if ((current_position < initial_pos) || (current_position + n > final_pos)) {
    throw ManagedStreamExceptions::OutOfBoundsException(
        n, initial_pos, final_pos, current_position);
  }
}

std::vector<std::byte> ManagedStream::get_n_bytes(uint64_t n) {
  dynamic_assert_access_bounds(n);
  std::vector<std::byte> temp_vector(n);
  ref_to_stream.read(reinterpret_cast<char*>(temp_vector.data()), n);
  return temp_vector;
}


std::byte ManagedStream::get_byte() {
  return get_bytes<1>()[0];
}
