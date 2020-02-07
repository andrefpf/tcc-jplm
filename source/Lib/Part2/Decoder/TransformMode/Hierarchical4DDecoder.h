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

/** \file     Hierarchical4DDecoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Murilo
 *  \date     2017-12-28
 */

#ifndef JPLM_LIB_PART2_DECODER_TRANSFORMMODE_HIERARCHICAL4DDECODER_H__
#define JPLM_LIB_PART2_DECODER_TRANSFORMMODE_HIERARCHICAL4DDECODER_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "Lib/Part2/Common/TransformMode/Hierarchical4DCodec.h"
#include "Lib/Part2/Common/TransformMode/ProbabilityModel.h"
#include "Lib/Part2/Decoder/TransformMode/ABACDecoder.h"


class Hierarchical4DDecoder : public Hierarchical4DCodec {
 private:
  HexadecaTreeFlag decode_segmentation_flag(int bitplane);

  int decode_coefficient(int bitplane);

 public:
  ABACDecoder entropy_decoder;

  Hierarchical4DDecoder(const ContiguousCodestreamCode& codestream_code)
      : entropy_decoder(codestream_code) {
  }

  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> get_transform_dimensions()
      const {
    return {mTransformLength_t, mTransformLength_s, mTransformLength_v,
        mTransformLength_u};
  }

  virtual ~Hierarchical4DDecoder() = default;

  void decode_block(int position_t, int position_s, int position_v,
      int position_u, int length_t, int length_s, int length_v, int length_u,
      int bitplane);

  PartitionFlag decode_partition_flag();

  int decode_integer(int precision);

  void start();

  virtual void reset_probability_models() override {
    Hierarchical4DCodec::reset_probability_models();
    entropy_decoder.start();
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_DECODER_TRANSFORMMODE_HIERARCHICAL4DDECODER_H__ */