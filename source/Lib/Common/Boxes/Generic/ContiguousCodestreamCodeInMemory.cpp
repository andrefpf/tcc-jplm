/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     ContiguousCodestreamCodeInMemory.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-06
 */

#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCodeInMemory.h"


uint64_t ContiguousCodestreamCodeInMemory::size() const noexcept {
  return bytes.size();
}


ContiguousCodestreamCodeInMemory::ContiguousCodestreamCodeInMemory() {
  bytes.reserve(100000);
}


ContiguousCodestreamCodeInMemory::ContiguousCodestreamCodeInMemory(
    uint64_t size) {
  bytes.reserve(size);
}


void ContiguousCodestreamCodeInMemory::push_byte(const std::byte byte) {
  bytes.emplace_back(byte);
  ++current_pos;
}


std::byte ContiguousCodestreamCodeInMemory::get_byte_at(
    const uint64_t pos) const {
  return bytes.at(pos);
}


std::byte ContiguousCodestreamCodeInMemory::get_next_byte() const {
  auto byte = bytes.at(current_pos);
  ++current_pos;
  return byte;
}


std::vector<std::byte> ContiguousCodestreamCodeInMemory::get_next_n_bytes(
    std::size_t n) const {
  auto bytes_vector = std::vector<std::byte>();
  bytes_vector.reserve(n);
  for (auto i = decltype(n){0}; i < n; ++i) {
    bytes_vector.push_back(this->get_next_byte());
  }
  // current_pos+=n;
  return bytes_vector;
}


std::byte ContiguousCodestreamCodeInMemory::peek_next_byte() const {
  return bytes.at(current_pos);
}


bool ContiguousCodestreamCodeInMemory::is_next_valid() const {
  return current_pos < bytes.size();
}


void ContiguousCodestreamCodeInMemory::rewind(
    std::size_t n_bytes_to_rewind) const {
  int64_t new_pos = current_pos - n_bytes_to_rewind;
  if (new_pos >= 0) {
    current_pos = static_cast<std::size_t>(new_pos);
  } else {
    current_pos = 0;
  }
}


ContiguousCodestreamCodeInMemory *ContiguousCodestreamCodeInMemory::clone()
    const {
  return new ContiguousCodestreamCodeInMemory(*this);
}


bool ContiguousCodestreamCodeInMemory::is_equal(
    const ContiguousCodestreamCode &other) const {
  if (this->size() != other.size()) {
    return false;
  }
  uint64_t counter = 0;
  for (const auto &byte : bytes) {
    if (byte != other.get_byte_at(counter)) {
      return false;
    }
  }
  return true;
}


std::ostream &ContiguousCodestreamCodeInMemory::write_to(
    std::ostream &stream) const {
  stream.write(reinterpret_cast<const char *>(bytes.data()), bytes.size());
  return stream;
}
