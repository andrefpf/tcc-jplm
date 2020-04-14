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

/** \file     ManagedStream.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-12     
 */

#ifndef JPLM_LIB_UTILS_STREAM_MANAGEDSTREAM_H__
#define JPLM_LIB_UTILS_STREAM_MANAGEDSTREAM_H__

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include "CommonExceptions.h"


class ManagedStream {
 protected:
  std::ifstream&
      ref_to_stream;  //it may be a good idea to change for a weak ptr
  const std::size_t initial_pos;
  const std::size_t final_pos;

 public:
  ManagedStream(
      std::ifstream& ref_to_stream, uint64_t initial_pos, uint64_t final_pos);
  ManagedStream(std::ifstream& ref_to_stream, uint64_t max_offset);

  ManagedStream get_sub_managed_stream(
      uint64_t initial_pos, uint64_t final_pos);


  ManagedStream get_sub_managed_stream(uint64_t max_offset);


  ManagedStream get_remaining_sub_managed_stream();


  std::size_t get_initial_pos() const noexcept;


  std::size_t get_final_pos() const noexcept;


  std::size_t get_current_pos() const noexcept;


  std::size_t tell() const noexcept;


  bool is_valid() const noexcept;


  bool is_valid(uint64_t index) const noexcept;


  ManagedStream& rewind();  //! places the stream at initial position


  ManagedStream& forward();  //! places the stream at final position


  ManagedStream& seek(int64_t offset,
      const std::ios_base::seekdir relative_to =
          std::ios_base::beg);  //! place the stream at


  void dynamic_assert_access_bounds(const uint64_t n) const;


  template<size_t N>
  std::vector<std::byte> get_bytes() {
    dynamic_assert_access_bounds(N);
    std::array<std::byte, N> temp_array;
    ref_to_stream.read(reinterpret_cast<char*>(temp_array.data()), N);
    return {std::begin(temp_array), std::end(temp_array)};
  }


  std::vector<std::byte> get_n_bytes(uint64_t n);


  std::byte get_byte();


  uint64_t get_length() const noexcept;


  ~ManagedStream() = default;
};

#endif /* end of include guard: JPLM_LIB_UTILS_STREAM_MANAGEDSTREAM_H__ */