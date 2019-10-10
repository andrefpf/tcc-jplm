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

/** \file     MuleEncoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

/*
* @Author: Ismael Seidel
* @Date:   2019-03-26 16:31:36
* @Last Modified by:   Ismael Seidel
* @Last Modified time: 2019-05-14 16:56:54
*/
#include "MuleEncoder.h"

// void MuleEncoder::open_lightfield() {

//     auto dimension = LightfieldDimension<std::size_t>(parameter_handler.number_of_vertical_views,
//                 parameter_handler.number_of_horizontal_views,
//                 3, 3);
//     auto config = LightfieldIOConfiguration(
//         parameter_handler.decoded_lightfield.string(),
//         dimension
//         );

//     lightfield = std::make_unique<LightFieldTransformMode<>>(config);
// }

MuleEncoder::MuleEncoder(ParameterHandler handler)
    : MuleCodec(handler), tp(parameter_handler.min_transform_length_t,
                              parameter_handler.min_transform_length_s,
                              parameter_handler.min_transform_length_v,
                              parameter_handler.min_transform_length_u) {
  auto dimension = LightfieldDimension<std::size_t>(
      parameter_handler.number_of_vertical_views,
      parameter_handler.number_of_horizontal_views, 3, 3);
  auto config = LightfieldIOConfiguration(
      parameter_handler.decoded_lightfield.string(), dimension);

  lightfield = std::make_unique<LightFieldTransformMode<>>(config);

  tp.mPartitionData.set_dimension(parameter_handler.transform_length_t,
      parameter_handler.transform_length_s,
      parameter_handler.transform_length_v,
      parameter_handler.transform_length_u);
  setup_hierarchical_4d_encoder();

  setup_transform_coefficients(true);

  write_initial_data_to_encoded_file();

  initialize_extension_lenghts();
}


void MuleEncoder::run_for_block_4d(const uint32_t channel,
    const int32_t level_shift, const LightfieldCoordinate<uint32_t>& position,
    const LightfieldDimension<uint32_t>& size) {
  auto block_4d = lightfield->get_block_4D_from(channel, position, size);
  block_4d += 0 - level_shift;
  hierarchical_4d_encoder.reset_probability_models();
  tp.rd_optimize_transform(
      block_4d, hierarchical_4d_encoder, parameter_handler.lambda);
  tp.encode_partition(hierarchical_4d_encoder, parameter_handler.lambda);
}


std::unique_ptr<ContiguousCodestreamCode>
MuleEncoder::get_contigous_codestream_code() {
  return hierarchical_4d_encoder.move_codestream_code_out();
}


void MuleEncoder::setup_hierarchical_4d_encoder() {
    hierarchical_4d_encoder.set_transform_dimension(
            {parameter_handler.transform_length_t, parameter_handler.transform_length_s,
      parameter_handler.transform_length_v, parameter_handler.transform_length_u});

  hierarchical_4d_encoder.create_temporary_buffer();

  hierarchical_4d_encoder.set_minimum_transform_dimension(
      {parameter_handler.min_transform_length_t, parameter_handler.min_transform_length_s,
      parameter_handler.min_transform_length_v, parameter_handler.min_transform_length_u});
  // std::tie(
  //     hierarchical_4d_encoder.mNumberOfVerticalViews,
  //     hierarchical_4d_encoder.mNumberOfHorizontalViews,
  //     hierarchical_4d_encoder.mNumberOfViewLines,
  //     hierarchical_4d_encoder.mNumberOfViewColumns) = lightfield->get_dimensions<int>();
  const auto& [T, S, V, U] = lightfield->get_dimensions<uint32_t>();
  hierarchical_4d_encoder.mNumberOfVerticalViews = T;
  hierarchical_4d_encoder.mNumberOfHorizontalViews = S;
  hierarchical_4d_encoder.mNumberOfViewLines = V;
  hierarchical_4d_encoder.mNumberOfViewColumns = U;

  hierarchical_4d_encoder.set_level_shift(
      std::pow(2, lightfield->get_views_bpp()) - 1);
}


void MuleEncoder::
    write_initial_data_to_encoded_file() {  //read_initial_data_from_encoded_file
  hierarchical_4d_encoder.write_initial_data();
}
