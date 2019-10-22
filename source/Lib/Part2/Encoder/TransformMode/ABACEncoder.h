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

/** \file     ABACEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_ABACENCODER_H__
#define JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_ABACENCODER_H__

#include <fstream>  // std::ofstream
#include <memory>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCode.h"
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCodeInMemory.h"
#include "Lib/Part2/Common/TransformMode/ABACCodec.h"
#include "Lib/Part2/Common/TransformMode/ProbabilityModel.h"
#include "Lib/Part2/Common/TransformMode/Markers.h"

  
class ABACEncoder : public ABACCodec {
 protected:
  mutable std::unique_ptr<ContiguousCodestreamCode> codestream_code;
  // ContiguousCodestreamCode& codestream_code;
  std::string filename;
  mutable int number_of_scalings; /*!< number of renormalizations performed */
  std::byte mask;
  std::byte byte_buffer;
  void mask_set_high_reset_low();
  void output_bit_pattern_according_to_condition(bool condition);
  template<bool bit>
  void output_n_bits(std::size_t n);
  template<bool bit>
  void output_bit();
  void output_bit(bool bit);
  void push_current_byte_to_codestream_code();

 public:
  ABACEncoder()
      : codestream_code(std::make_unique<ContiguousCodestreamCodeInMemory>()),
        number_of_scalings(0), mask(std::byte{0x01}),
        byte_buffer(std::byte{0}) {
  }

  virtual ~ABACEncoder() = default;
  void start(const std::string& filename);
  void encode_bit(bool bit, const ProbabilityModel& probability_model);

    template<bool bit>
    void encode_bit(const ProbabilityModel& probability_model) {
        uint64_t length_0 = (((mHigh - mLow + 1) * probability_model.get_frequency_of_zeros()) /
                             probability_model.get_frequency_of_ones());

        if constexpr (bit) {  //1
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

  ContiguousCodestreamCode& get_ref_to_codestream_code() const;

  void reset() {
    number_of_scalings = 0;
    mask=std::byte{0x01};
    byte_buffer=std::byte{0};
    mLow=0;
    mHigh=MAXINT;
    number_of_bits_in_byte=0;
    mBitBuffer=0;
  }


  // void flush_byte() {
  //   std::cout << "called flush" << std::endl;
  // }


  void flush_byte() {
    std::cout << "called flush" << std::endl;
    number_of_scalings++;
    output_bit_pattern_according_to_condition(mLow >= SECOND_MSB_MASK);
    push_current_byte_to_codestream_code();
    // codestream_code->push_byte(std::byte{0xff}); //dummy byte 1
    // codestream_code->push_byte(std::byte{0xa4}); //dummy byte 2
    // codestream_code->push_byte(byte_buffer);
    reset();
  }


  std::unique_ptr<ContiguousCodestreamCode>&& move_codestream_code_out() {
    flush_byte();
    auto eoc_bytes = Markers::get_bytes(Marker::EOC);
    codestream_code->push_byte(eoc_bytes[0]);
    codestream_code->push_byte(eoc_bytes[1]);
    return std::move(codestream_code);
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_ABACENCODER_H__ */