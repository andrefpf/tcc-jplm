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

/** \file     PartitionDecoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#include "PartitionDecoder.h"


Block4D PartitionDecoder::decode_partition(
    Hierarchical4DDecoder &hierarchical_decoder, const LightfieldDimension<uint32_t>& size) {

  int position[] = {0, 0, 0, 0};

  uint32_t length[4];
  std::tie(length[0], length[1], length[2], length[3]) = size.as_tuple();
  
  mPartitionData = Block4D(size);
  mPartitionData.fill_with_zeros();

  hierarchical_decoder.mInferiorBitPlane = hierarchical_decoder.decode_integer(MINIMUM_BITPLANE_PRECISION);
  
  decode_partition(position, length, hierarchical_decoder);

  return Block4D(std::move(mPartitionData));
}


void PartitionDecoder::decode_partition(
    int *position, uint32_t *length, Hierarchical4DDecoder &hierarchical_decoder) {
  switch (hierarchical_decoder.decode_partition_flag()) {
    case PartitionFlag::transform: {
      return decode_transform_partition(position, length, hierarchical_decoder);
    }
    case PartitionFlag::spatialSplit: {
      int new_position[] = {position[0], position[1], position[2], position[3]};
      uint32_t new_length[] = {length[0], length[1], length[2] / 2, length[3] / 2};

      //Decode four spatial subblocks
      decode_partition(new_position, new_length, hierarchical_decoder);

      new_position[3] = position[3] + length[3] / 2;
      new_length[3] = length[3] - length[3] / 2;

      decode_partition(new_position, new_length, hierarchical_decoder);

      new_position[2] = position[2] + length[2] / 2;
      new_length[2] = length[2] - length[2] / 2;

      decode_partition(new_position, new_length, hierarchical_decoder);

      new_position[3] = position[3];
      new_length[3] = length[3] / 2;

      decode_partition(new_position, new_length, hierarchical_decoder);
      return;
    }
    case PartitionFlag::viewSplit: {
      int new_position[] = {position[0], position[1], position[2], position[3]};
      uint32_t new_length[] = {length[0] / 2, length[1] / 2, length[2], length[3]};

      //Decode four view subblocks
      decode_partition(new_position, new_length, hierarchical_decoder);

      new_position[1] = position[1] + length[1] / 2;
      new_length[1] = length[1] - length[1] / 2;

      decode_partition(new_position, new_length, hierarchical_decoder);

      new_position[0] = position[0] + length[0] / 2;
      new_length[0] = length[0] - length[0] / 2;

      decode_partition(new_position, new_length, hierarchical_decoder);

      new_position[1] = position[1];
      new_length[1] = length[1] / 2;

      decode_partition(new_position, new_length, hierarchical_decoder);
      return;
    }
  }
}


void PartitionDecoder::decode_transform_partition(
    int *position, uint32_t *length, Hierarchical4DDecoder &hierarchical_decoder) {
  hierarchical_decoder.mSubbandLF.set_dimension(
      length[0], length[1], length[2], length[3]);
  hierarchical_decoder.mSubbandLF.fill_with_zeros();
  hierarchical_decoder.decode_block(0, 0, 0, 0, length[0], length[1], length[2],
      length[3], hierarchical_decoder.mSuperiorBitPlane);
  DCT4DBlock dctblock(std::move(
      hierarchical_decoder
          .mSubbandLF));  //uses move in the initialization of transformed block.
  hierarchical_decoder.mSubbandLF =
      dctblock.inverse();  //hopefully using move (copy elision)
  mPartitionData.copy_sub_block_from(hierarchical_decoder.mSubbandLF, 0, 0, 0,
      0, position[0], position[1], position[2], position[3]);
}
