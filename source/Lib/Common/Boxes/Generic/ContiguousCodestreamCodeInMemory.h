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

/** \file     ContiguousCodestreamCodeInMemory.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-29
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODEINMEMORY_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODEINMEMORY_H__

#include <iostream>
#include <vector>
#include "ContiguousCodestreamCode.h"


class ContiguousCodestreamCodeInMemory : public ContiguousCodestreamCode {
 protected:
  std::vector<std::byte> bytes;
  mutable std::size_t current_pos = 0;

 public:
  virtual uint64_t size() const noexcept override {
    return bytes.size();
  }

  ContiguousCodestreamCodeInMemory() {
    bytes.reserve(100000);
  }


  ContiguousCodestreamCodeInMemory(uint64_t size) {
    bytes.reserve(size);
  }


  ContiguousCodestreamCodeInMemory(const std::vector<std::byte>& bytes)
      : bytes(bytes) {
  }


  ContiguousCodestreamCodeInMemory(std::vector<std::byte>&& bytes)
      : bytes(std::move(bytes)) {
  }


  ContiguousCodestreamCodeInMemory(
      const ContiguousCodestreamCodeInMemory& other)
      : bytes(other.bytes), current_pos(other.current_pos) {
  }


  ContiguousCodestreamCodeInMemory(ContiguousCodestreamCodeInMemory&& other)
      : bytes(std::move(other.bytes)), current_pos(other.current_pos) {
  }


  virtual ~ContiguousCodestreamCodeInMemory() = default;


  virtual void push_byte(const std::byte byte) override {
    bytes.emplace_back(byte);
    ++current_pos;
  }


  virtual std::byte get_byte_at(const uint64_t pos) const override {
    return bytes.at(pos);
  }


  virtual std::byte get_next_byte() const override {
    auto byte = bytes.at(current_pos);
    ++current_pos;
    return byte;
  }


  virtual std::byte peek_next_byte() const override {
    return bytes.at(current_pos);
  }


  virtual bool is_next_valid() const override {
    return current_pos < bytes.size(); 
  }


  virtual void rewind(std::size_t n_bytes_to_rewind) const override {
    int64_t new_pos = current_pos - n_bytes_to_rewind;
    if(new_pos >= 0) {
      current_pos = static_cast<std::size_t>(new_pos);
    } else {
      current_pos = 0;
    }
  }


  virtual ContiguousCodestreamCodeInMemory* clone() const override {
    return new ContiguousCodestreamCodeInMemory(*this);
  }


  virtual bool is_equal(const ContiguousCodestreamCode& other) const override {
    if (this->size() != other.size()) {
      return false;
    }
    uint64_t counter = 0;
    for (const auto& byte : bytes) {
      if (byte != other.get_byte_at(counter)) {
        return false;
      }
    }
    return true;
  }


  virtual std::ostream& write_to(std::ostream& stream) const override {
    stream.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    return stream;
  }
};

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_CONTIGUOUSCODESTREAMCODEINMEMORY_H__ */