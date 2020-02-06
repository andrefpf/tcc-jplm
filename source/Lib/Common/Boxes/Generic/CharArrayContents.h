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

/** \file     CharArrayContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_CHARARRAYCONTENTS_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_CHARARRAYCONTENTS_H__

#include "Lib/Common/Boxes/InMemoryDBox.h"


class CharArrayContents : public InMemoryDBox {
 protected:
  std::vector<uint8_t> chars;

 public:
  explicit CharArrayContents(const std::vector<uint8_t>& array) : chars(array) {
  }


  explicit CharArrayContents(std::vector<uint8_t>&& array) : chars(std::move(array)) {
  }


  CharArrayContents(const CharArrayContents& other)
      : chars(other.get_const_ref_to_vector()) {
  }


  CharArrayContents(CharArrayContents&& other)
      : chars(std::move(other.chars)){};


  ~CharArrayContents() = default;


  uint64_t size() const noexcept override {
    return chars.size();
  }


  CharArrayContents* clone() const override {
    return new CharArrayContents(*this);
  }


  const std::vector<uint8_t>& get_const_ref_to_vector() const {
    return chars;
  }


  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other = dynamic_cast<const CharArrayContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const CharArrayContents& other) const {
    return this->chars == other.chars;
  }


  bool operator!=(const CharArrayContents& other) const {
    return !this->operator==(other);
  }


  std::vector<std::byte> get_bytes() const noexcept override {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->size());
    for (const auto& value : chars) {
      bytes.emplace_back(std::byte{value});
    }
    return bytes;
  }
};

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_CHARARRAYCONTENTS_H__ */