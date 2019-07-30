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

/** \file     BoxDataHolder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-23
 */

#ifndef JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__
#define JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__

#include <ostream>
#include <vector>
#include <algorithm>    // std::reverse
#include "Lib/Utils/BinaryTools.h"


template<typename T>
class BoxDataHolder {
 protected:
  T value;

 public:
  BoxDataHolder() : value(0){};


  BoxDataHolder(const T& value) : value(value){};


  virtual ~BoxDataHolder() {
  }


  T get_value() const {
    return value;
  }


  void set_value(T value) {
    this->value = value;
  }

  bool is_equal(const BoxDataHolder& other) const noexcept {
    if(this->value == other.value) {
      return true;
    }
    return false;
  }

  std::vector<uint8_t> get_bytes() const noexcept {
    auto n_bytes = sizeof(T); 
    auto bytes = std::vector<uint8_t>();
    if (n_bytes == 1) {
      bytes.push_back(value);
      return bytes;
    }
    bytes.reserve(n_bytes);
    auto ptr_to_byte = reinterpret_cast<const uint8_t*>(&value);

    for(auto i=decltype(n_bytes){0}; i<n_bytes;++i) {
      bytes.push_back(*ptr_to_byte);
      ++ptr_to_byte;
    }

    if constexpr (BinaryTools::using_little_endian()) {
      std::reverse(bytes.begin(), bytes.end());  
    }
    return bytes;
  }

};


template<typename T>
std::ostream& operator<<(
    std::ostream& stream, const BoxDataHolder<T>& box_data_holder) {
  auto bytes = box_data_holder.get_bytes();
  for(const auto& byte: bytes) {
    stream << byte;
  }
  return stream;
}

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXDATAHOLDER_H__ */