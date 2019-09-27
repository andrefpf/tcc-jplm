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

class ABACEncoder : public ABACCodec {
 protected:
  std::unique_ptr<ContiguousCodestreamCode> codestream_code;
  std::string filename;
  int number_of_scalings; /*!< number of renormalizations performed */
  std::byte mask;
  std::byte byte_buffer;
  void mask_set_high_reset_low();
  void output_bit_pattern_according_to_condition(bool condition);
  template<bool bit>
  void output_n_bits(std::size_t n);
  template<bool bit>
  void output_bit();
  void output_bit(bool bit);

 public:
  ABACEncoder()
      : codestream_code(std::make_unique<ContiguousCodestreamCodeInMemory>()),
        number_of_scalings(0), mask(std::byte{0x01}),
        byte_buffer(std::byte{0}) {
  }
  virtual ~ABACEncoder() = default;
  void start(const std::string& filename);
  void encode_bit(bool bit, const ProbabilityModel& probability_model);
  void finish() override;
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_ABACENCODER_H__ */