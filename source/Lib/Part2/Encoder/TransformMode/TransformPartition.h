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

/** \file     TransformPartition.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Murilo Bresciani
 *  \date     2019-09-26
 */

#ifndef JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_TRANSFORMPARTITION_H__
#define JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_TRANSFORMPARTITION_H__


#include "Lib/Part2/Common/TransformMode/DCT4DBlock.h"
#include "Lib/Part2/Encoder/TransformMode/CommonExceptions.h"
#include "Lib/Part2/Encoder/TransformMode/Hierarchical4DEncoder.h"


class TransformPartition {
 private:
  std::vector<PartitionFlag> partition_code;
  int mPartitionCodeIndex; /*!< Scan index for the partition tree code string */
  int mlength_t_min, mlength_s_min, mlength_v_min, mlength_u_min;
  bool
      mEvaluateOptimumBitPlane; /*!< Toggles the optimum bit plane evaluation procedure on and off */

 public:
  Block4D mPartitionData; /*!< DCT of all subblocks of the partition */
  TransformPartition(
      int length_t_min, int length_s_min, int length_v_min, int length_u_min);
  TransformPartition(
      const LightfieldDimension<uint32_t> &minimum_transform_dimensions);
  ~TransformPartition() = default;
  RDCostResult rd_optimize_transform(
      Block4D &inputBlock, Hierarchical4DEncoder &entropyCoder, double lambda);
  RDCostResult rd_optimize_transform(Block4D &inputBlock,
      Block4D &transformedBlock, const std::tuple<int, int, int, int> &position,
      const std::tuple<int, int, int, int> &lengths,
      Hierarchical4DEncoder &entropyCoder, double lambda,
      std::vector<PartitionFlag> &partition_code);
  void encode_partition(Hierarchical4DEncoder &entropyCoder, double lambda);
  RDCostResult encode_partition(const std::tuple<int, int, int, int> &position,
      const std::tuple<int, int, int, int> &lengths,
      Hierarchical4DEncoder &entropyCoder, double lambda);

  const std::vector<PartitionFlag> &get_partition_code() const;
  void show_partition_codes_and_inferior_bit_plane() const;
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_TRANSFORMPARTITION_H__ */