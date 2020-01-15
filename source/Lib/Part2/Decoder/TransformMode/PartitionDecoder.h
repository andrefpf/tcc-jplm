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

/** \file     PartitionDecoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#ifndef JPLM_LIB_PART2_DECODER_TRANSFORMMODE_PARTITIONDECODER_H__
#define JPLM_LIB_PART2_DECODER_TRANSFORMMODE_PARTITIONDECODER_H__

#include <math.h>
#include <string.h>
#include <memory>
#include <vector>
#include "Lib/Part2/Common/TransformMode/DCT4DBlock.h"
#include "Lib/Part2/Decoder/TransformMode/Hierarchical4DDecoder.h"

class PartitionDecoder {
 private:
  static constexpr auto MINIMUM_BITPLANE_PRECISION = 5;
  void decode_transform_partition(uint16_t channel, int *position,
      uint32_t *length, Hierarchical4DDecoder &hierarchical_decoder);
  void decode_partition(uint16_t channel, int *position, uint32_t *length,
      Hierarchical4DDecoder &entropyDecoder);

 protected:
  std::vector<double> scaling_factors;

 public:
  Block4D mPartitionData; /*!< DCT of all subblocks of the partition */
  PartitionDecoder() = default;
  ~PartitionDecoder() = default;
  Block4D decode_partition(uint16_t channel,
      Hierarchical4DDecoder &entropyDecoder,
      const LightfieldDimension<uint32_t> &size);


  void set_colour_component_scaling_factor(
      const std::size_t colour_component_index, double scaling_factor) {
    this->scaling_factors[colour_component_index] = scaling_factor;
  }


  void set_number_of_colour_components(uint16_t number_of_colour_components) {
    this->scaling_factors =
        std::vector<double>(number_of_colour_components, 1.0);
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_DECODER_TRANSFORMMODE_PARTITIONDECODER_H__ */