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

/** \file     ABACDecoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Murilo Bresciani 
 *  \date     2019-09-26
 */

#include "ABACDecoder.h"
#include <iostream>

//from http://graphics.stanford.edu/~seander/bithacks.html
inline std::byte swap_bits(std::byte byte) {
    auto b = std::to_integer<uint8_t>(byte);
    b = ((b * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
    return std::byte{b};
}


inline void ABACDecoder::init_tag() {
   if (codestream_code.is_next_valid()) {
       auto ms_byte = codestream_code.get_next_byte();
       auto ls_byte = codestream_code.get_next_byte();
       // print_byte(ms_byte);
       // print_byte(ls_byte);
       tag = std::to_integer<uint16_t>(swap_bits(ms_byte));
       tag<<=8;
       tag |= std::to_integer<uint16_t>(swap_bits(ls_byte));
       // std::cout << tag << std::endl;
   }
}


inline void ABACDecoder::update_tag() {
  tag <<= 1;
  tag += get_next_bit_from_codestream();
}


inline void ABACDecoder::update_low_high_and_tag() {
  mLow <<= 1;
  mHigh <<= 1;
  ++mHigh;
  update_tag();
}


inline void ABACDecoder::mask_low_high_and_tag() {
  mLow ^= MSB_MASK;
  mHigh ^= MSB_MASK;
  tag ^= MSB_MASK;
}


void ABACDecoder::start() {
  if (codestream_code.peek_next_byte() == std::byte{0xff}) {
    [[maybe_unused]] auto byte = codestream_code.get_next_byte();
  }
  if (codestream_code.peek_next_byte() == std::byte{0xa4}) {
    [[maybe_unused]] auto byte = codestream_code.get_next_byte();
  }

  number_of_bits_in_byte = 0;
  mLow = 0;
  mHigh = MAXINT;
  tag = 0;

  init_tag();
}

bool ABACDecoder::decode_bit(const ProbabilityModel& mPmodel) {
  uint64_t acumFreq_0 = mPmodel.get_frequency_of_zeros();
  uint64_t acumFreq_1 = mPmodel.get_frequency_of_ones();
  uint64_t threshold =
      (((tag - mLow + 1) * acumFreq_1 - 1) / (mHigh - mLow + 1));
  uint64_t length_0 = (((mHigh - mLow + 1) * acumFreq_0) / acumFreq_1);

  bool bit_decoded;

  if (threshold < acumFreq_0) {
    bit_decoded = 0;
    mHigh = mLow + length_0 - 1;
  } else {
    bit_decoded = 1;
    mLow = mLow + length_0;
  }

  while (((mLow & MSB_MASK) == (mHigh & MSB_MASK)) ||
         ((mLow >= SECOND_MSB_MASK) && (mHigh < (TWO_MSBS_MASK)))) {
    if ((mLow & MSB_MASK) == (mHigh & MSB_MASK)) {
      update_low_high_and_tag();
    }
    if ((mLow >= SECOND_MSB_MASK) && (mHigh < (TWO_MSBS_MASK))) {
      update_low_high_and_tag();
      mask_low_high_and_tag();
    }
  }

  return bit_decoded;
}


//reads lsb first
bool ABACDecoder::get_next_bit_from_codestream() {
  if (number_of_bits_in_byte == 0) {
    number_of_bits_in_byte = 8;
    if (codestream_code.is_next_valid()) {
      byte_buffer = codestream_code.get_next_byte();
    }
  }
  auto byte_copy = std::to_integer<uint8_t>(byte_buffer);
  byte_buffer>>= 1;
  --number_of_bits_in_byte;
  return byte_copy & 0x01;
}
