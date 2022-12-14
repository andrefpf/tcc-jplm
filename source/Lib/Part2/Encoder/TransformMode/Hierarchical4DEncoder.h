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

/** \file     Hierarchical4DEncoder.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Murilo Bresciani
 *  \author   Murilo
 *  \date     2019-09-26
 */

#ifndef JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_HIERARCHICAL4DENCODER_H__
#define JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_HIERARCHICAL4DENCODER_H__

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <memory>
#include <numeric>
#include <tuple>
#include "Lib/Part2/Common/TransformMode/Block4D.h"
#include "Lib/Part2/Common/TransformMode/Hierarchical4DCodec.h"
#include "Lib/Part2/Common/TransformMode/LightFieldConfigurationMarkerSegment.h"
#include "Lib/Part2/Common/TransformMode/ProbabilityModelsHandler.h"
#include "Lib/Part2/Encoder/TransformMode/ABACEncoder.h"
#include "Lib/Part2/Encoder/TransformMode/RDCostResult.h"
#include "Lib/Utils/Stream/BinaryTools.h"
//#include "Lib/Common/JPLMEncoderConfigurationLightField4DTransformMode.h"

class Hierarchical4DEncoder : public Hierarchical4DCodec {
 private:
  static constexpr unsigned int ONES_MASK =
      std::numeric_limits<unsigned int>::max();
  std::unique_ptr<double[]> temporary_buffer;
  double total_energy_sum = 0.0;

 public:
  ABACEncoder mEntropyCoder;
  ProbabilityModelsHandler optimization_probability_models;
  std::vector<HexadecaTreeFlag> hexadecatree_flags;

  Hierarchical4DEncoder()
      : optimization_probability_models(probability_models) {
  }

  virtual ~Hierarchical4DEncoder() = default;

  bool get_mSubbandLF_significance(uint32_t threshold,
      const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& range) const;
  void reset_probability_models() override;
  void encode_coefficient(int coefficient, uint8_t bitplane);
  void encode_segmentation_lowerBitPlane_flag(uint8_t bitplane);
  void encode_segmentation_splitBlock_flag(uint8_t bitplane);
  void encode_segmentation_zeroBlock_flag(uint8_t bitplane);
  void encode_partition_transform_flag();
  void encode_partition_spatialSplit_flag();
  void encode_partition_viewSplit_flag();
  void encode_inferior_bit_plane_value();
  void encode_hexadecatree(int position_t, int position_s, int position_v,
      int position_u, int length_t, int length_s, int length_v, int length_u,
      uint8_t bitplane, int& flagIndex);
  RDCostResult encode_sub_block(double lambda);


  RDCostResult get_rd_for_below_inferior_bit_plane(
      const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& lengths);

  RDCostResult get_rd_for_unitary_block(
      const LightfieldCoordinate<uint32_t>& position, double lambda,
      uint8_t bitplane);

  RDCostResult rd_optimize_hexadecatree(
      const std::tuple<int, int, int, int>& position,
      const std::tuple<int, int, int, int>& lengths, double lambda,
      uint8_t bitplane, std::vector<HexadecaTreeFlag>& hexadecatree_flags);
  int get_optimum_bit_plane(double lambda);
  void load_optimizer_state();
  void set_optimization_model(const ProbabilityModelsHandler& model);
  void create_temporary_buffer();


  void show_inferior_bit_plane() const;


  void write_marker(Marker marker) {
    auto& codestream_code = mEntropyCoder.get_ref_to_codestream_code();
    auto bytes = Markers::get_bytes(marker);
    for (const auto& byte : bytes) {
      codestream_code.push_byte(byte);
    }
  }


  void write_lightfield_configuration_marker_segment(
      const LightFieldConfigurationMarkerSegment&
          lightfield_configuration_marker_segment) {
    auto& codestream_code = mEntropyCoder.get_ref_to_codestream_code();

    auto bytes = lightfield_configuration_marker_segment.get_bytes();
    for (const auto& byte : bytes) {
      codestream_code.push_byte(byte);
    }
  }


  ContiguousCodestreamCode& get_ref_to_codestream_code() const;

  std::unique_ptr<ContiguousCodestreamCode>&& move_codestream_code_out();
};

#endif /* end of include guard: JPLM_LIB_PART2_ENCODER_TRANSFORMMODE_HIERARCHICAL4DENCODER_H__ */