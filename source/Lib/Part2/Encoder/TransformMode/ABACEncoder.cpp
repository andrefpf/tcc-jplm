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

/** \file     ABACEncoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Murilo
 *  \date     2019-09-26
 */


#include "ABACEncoder.h"
#include "assert.h"


void ABACEncoder::mask_set_high_reset_low() {
  mLow <<= 1;
  mLow &= RESET_LSB_MASK;
  mLow &= MAXINT;

  mHigh <<= 1;
  mHigh |= SET_LSB_MASK;
  mHigh &= MAXINT;
}


void ABACEncoder::output_bit_pattern_according_to_condition(bool condition) {
  if (condition) {
    output_bit<1>();
    output_n_bits<0>(number_of_scalings);
  } else {
    output_bit<0>();
    output_n_bits<1>(number_of_scalings);
  }
  number_of_scalings = 0;
}


/*! encodes a binary symbol using the given probability model */
void ABACEncoder::encode_bit(bool bit, const ProbabilityModel& probability_model) {
  uint64_t length_0 = (((mHigh - mLow + 1) * probability_model.get_frequency_of_zeros()) /
                       probability_model.get_frequency_of_ones());

  if (bit) {  //1
    mLow += length_0;
  } else {  //0
    mHigh = mLow + length_0 - 1;
  }

  while (
      ((mLow & MSB_MASK) == (mHigh & MSB_MASK)) ||
      ((mLow >= SECOND_MSB_MASK) && (mHigh < (MSB_MASK + SECOND_MSB_MASK)))) {
    if ((mLow & MSB_MASK) == (mHigh & MSB_MASK)) {
      output_bit_pattern_according_to_condition((mLow & MSB_MASK) != 0);
      mask_set_high_reset_low();
    }
    if ((mLow >= SECOND_MSB_MASK) && (mHigh < (MSB_MASK + SECOND_MSB_MASK))) {
      number_of_scalings++;
      mask_set_high_reset_low();
      mLow ^= MSB_MASK;
      mHigh ^= MSB_MASK;
    }
  }
}


template<bool bit>
void ABACEncoder::output_n_bits(std::size_t n) {
  while (n > 0) {
    output_bit<bit>();
    n--;
  }
}


/*! write remaining bits in the outputfile */
void ABACEncoder::finish() {  //flush
  number_of_scalings++;
  output_bit_pattern_according_to_condition(mLow >= SECOND_MSB_MASK);
  codestream_code->push_byte(byte_buffer);
  // write_to_file(filename, *codestream_code);
}


ContiguousCodestreamCode& ABACEncoder::get_ref_to_codestream_code() const {
    return *codestream_code;
}


template<bool bit>
void ABACEncoder::output_bit() {
  if constexpr (bit) {  //==1
    byte_buffer |= mask;
  }
  mask <<= 1;
  if (mask == std::byte{0x0}) {
    mask = std::byte{0x01};
    codestream_code->push_byte(byte_buffer);
    byte_buffer = std::byte{0};
  }
}


void ABACEncoder::output_bit(bool bit) {
  if (bit) {
    return output_bit<true>();
  }
  return output_bit<false>();
}