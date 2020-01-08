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

/** \file     TransformPartition.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#include "TransformPartition.h"


TransformPartition::TransformPartition(
    int length_t_min, int length_s_min, int length_v_min, int length_u_min)
    : mlength_t_min(length_t_min), mlength_s_min(length_s_min),
      mlength_v_min(length_v_min), mlength_u_min(length_u_min) {
}

TransformPartition::TransformPartition(const LightfieldDimension<uint32_t>& minimum_transform_dimensions) :
        mlength_t_min(minimum_transform_dimensions.get_t()),
        mlength_s_min(minimum_transform_dimensions.get_s()),
        mlength_v_min(minimum_transform_dimensions.get_v()),
        mlength_u_min(minimum_transform_dimensions.get_u()) {
}

/*! Evaluates the Lagrangian cost of the optimum multiscale transform for the input block as well as the transformed block */
void TransformPartition::rd_optimize_transform(Block4D &input_block,
    Hierarchical4DEncoder &hierarchical_4d_encoder, double lambda) {
  // double scaled_lambda = lambda*input_block.get_number_of_elements();
  double scaled_lambda = lambda * hierarchical_4d_encoder.get_number_of_elements_in_transform();

  partition_code.clear();
  mEvaluateOptimumBitPlane = true;

  //copies the current hierarchical_4d_encoder arithmetic model to the optimizer model.
  hierarchical_4d_encoder.load_optimizer_state();

  auto lengths = input_block.get_dimension();
  Block4D transformed_block;
  rd_optimize_transform(input_block, transformed_block, {0, 0, 0, 0}, lengths,
      hierarchical_4d_encoder, scaled_lambda, partition_code);

  mPartitionData = std::move(transformed_block);

  //Restores state since the encoder will reevaluate it
  hierarchical_4d_encoder.load_optimizer_state();

  auto mPartitionCode = std::string("");
  for (const auto &flag : partition_code) {
    switch (flag) {
      case PartitionFlag::transform:
          mPartitionCode+='T';
        break;
      case PartitionFlag::spatialSplit:
        mPartitionCode+='S';
        break;
      case PartitionFlag::viewSplit:
          mPartitionCode+='V';
        break;
    }
  }

std::cout << "Partition code: " << mPartitionCode << "\n";
  std::cout << "Inferior bit plane value: " << static_cast<uint32_t>(hierarchical_4d_encoder.get_inferior_bit_plane()) << "\n";
}


void scale_block(Block4D& transformed_block, double scale_factor) {
  
}


double TransformPartition::rd_optimize_transform(Block4D &input_block,
    Block4D &transformed_block, const std::tuple<int, int, int, int> &position,
    const std::tuple<int, int, int, int> &lengths,
    Hierarchical4DEncoder &hierarchical_4d_encoder, double lambda,
    std::vector<PartitionFlag> &partition_code) {
  using LF = LightFieldDimension;
  /*! returns the Lagrangian cost of one step of the optimization of the multiscale transform for the input
   * block as well as the transformed block */
  // std::cerr << "in rd_optimize_transform (" << mEvaluateOptimumBitPlane << ")" << std::endl;
  //saves the current hierarchical_4d_encoder arithmetic model to initial_model.
  auto initial_model = hierarchical_4d_encoder.optimization_probability_models;
  std::vector<PartitionFlag> partition_code_spatialSplit;
  std::vector<PartitionFlag> partition_code_viewSplit;

  //copy the input_block to block_0 and apply transformation using the appropriate scale from mt
  Block4D block_0;
  block_0.set_dimension(lengths);
  block_0.copy_sub_block_from(input_block, position);

  //substituted the multiscale transform call for this new one
  DCT4DBlock dctblock(block_0);
  dctblock.swap_data_with_block(block_0);

  //copy the transformed input block to hierarchical_4d_encoder.mSubbandLF
  hierarchical_4d_encoder.mSubbandLF =
      block_0;  //copy, its not possible to move...

  if (mEvaluateOptimumBitPlane) {
      hierarchical_4d_encoder.set_inferior_bit_plane(hierarchical_4d_encoder.get_optimum_bit_plane(lambda));
    hierarchical_4d_encoder.load_optimizer_state();
    mEvaluateOptimumBitPlane = false;
  }

  //call rd_optimize_hexadecatree method from hierarchical_4d_encoder to evaluate J0
  hierarchical_4d_encoder.hexadecatree_flags.clear();

  auto probability_model_for_transform = initial_model;
  auto probability_model_for_spatialSplit = initial_model;
  auto probability_model_for_viewSplit = initial_model;

  hierarchical_4d_encoder.set_optimization_model(
      probability_model_for_transform);

  double J0 =
      lambda +
      std::get<0>(hierarchical_4d_encoder.rd_optimize_hexadecatree({0, 0, 0, 0},
          {block_0.mlength_t, block_0.mlength_s, block_0.mlength_v,
              block_0.mlength_u},
          lambda, hierarchical_4d_encoder.get_superior_bit_plane(),
          hierarchical_4d_encoder.hexadecatree_flags));

  double bestJ = J0;
  auto partition_mode = PartitionFlag::transform;

  //Restores the current arithmetic model using current_model.
  probability_model_for_transform =
      hierarchical_4d_encoder.optimization_probability_models;
  hierarchical_4d_encoder.set_optimization_model(
      probability_model_for_spatialSplit);

  Block4D transformed_block_S;

  if ((std::get<LF::U>(lengths) >= 2 * mlength_u_min) &&
      (std::get<LF::V>(lengths) >= 2 * mlength_v_min)) {
    std::vector<PartitionFlag> partition_mode_s00;
    Block4D transformed_block_S00;
    double JS =
        2.0 * lambda +
        rd_optimize_transform(input_block, transformed_block_S00, position,
            {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
                std::get<LF::V>(lengths) / 2, std::get<LF::U>(lengths) / 2},
            hierarchical_4d_encoder, lambda, partition_mode_s00);

    if (JS < bestJ) {
      std::vector<PartitionFlag> partition_mode_s01;
      Block4D transformed_block_S01;
      JS += rd_optimize_transform(input_block, transformed_block_S01,
          {std::get<LF::T>(position), std::get<LF::S>(position),
              std::get<LF::V>(position),
              std::get<LF::U>(position) + std::get<LF::U>(lengths) / 2},
          {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
              std::get<LF::V>(lengths) / 2,
              std::get<LF::U>(lengths) - std::get<LF::U>(lengths) / 2},
          hierarchical_4d_encoder, lambda, partition_mode_s01);

      if (JS < bestJ) {
        std::vector<PartitionFlag> partition_mode_s11;
        Block4D transformed_block_S11;
        JS += rd_optimize_transform(input_block, transformed_block_S11,
            {std::get<LF::T>(position), std::get<LF::S>(position),
                std::get<LF::V>(position) + std::get<LF::V>(lengths) / 2,
                std::get<LF::U>(position) + std::get<LF::U>(lengths) / 2},
            {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
                std::get<LF::V>(lengths) - std::get<LF::V>(lengths) / 2,
                std::get<LF::U>(lengths) - std::get<LF::U>(lengths) / 2},
            hierarchical_4d_encoder, lambda, partition_mode_s11);

        if (JS < bestJ) {
          std::vector<PartitionFlag> partition_mode_s10;
          Block4D transformed_block_S10;
          JS += rd_optimize_transform(input_block, transformed_block_S10,
              {std::get<LF::T>(position), std::get<LF::S>(position),
                  std::get<LF::V>(position) + std::get<LF::V>(lengths) / 2,
                  std::get<LF::U>(position)},
              {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
                  std::get<LF::V>(lengths) - std::get<LF::V>(lengths) / 2,
                  std::get<LF::U>(lengths) / 2},
              hierarchical_4d_encoder, lambda, partition_mode_s10);

          if (JS < bestJ) {
            bestJ = JS;
            partition_mode = PartitionFlag::spatialSplit;
            partition_code_viewSplit.reserve(
                partition_code_spatialSplit.size() + partition_mode_s00.size() +
                partition_mode_s01.size() + partition_mode_s11.size() +
                partition_mode_s10.size());
            partition_code_spatialSplit.insert(
                partition_code_spatialSplit.end(), partition_mode_s00.begin(),
                partition_mode_s00.end());
            partition_code_spatialSplit.insert(
                partition_code_spatialSplit.end(), partition_mode_s01.begin(),
                partition_mode_s01.end());
            partition_code_spatialSplit.insert(
                partition_code_spatialSplit.end(), partition_mode_s11.begin(),
                partition_mode_s11.end());
            partition_code_spatialSplit.insert(
                partition_code_spatialSplit.end(), partition_mode_s10.begin(),
                partition_mode_s10.end());
            probability_model_for_spatialSplit =
                hierarchical_4d_encoder.optimization_probability_models;

            transformed_block_S.set_dimension(lengths);

            transformed_block_S.copy_sub_block_from(transformed_block_S10, 0, 0,
                0, 0, 0, 0, std::get<LF::V>(lengths) / 2, 0);
            transformed_block_S.copy_sub_block_from(transformed_block_S11, 0, 0,
                0, 0, 0, 0, std::get<LF::V>(lengths) / 2,
                std::get<LF::U>(lengths) / 2);
            transformed_block_S.copy_sub_block_from(transformed_block_S01, 0, 0,
                0, 0, 0, 0, 0, std::get<LF::U>(lengths) / 2);
            transformed_block_S.copy_sub_block_from(
                transformed_block_S00, 0, 0, 0, 0);
          }
        }
      }
    }
  }

  hierarchical_4d_encoder.set_optimization_model(
      probability_model_for_viewSplit);
  Block4D transformed_block_V;

  if ((std::get<LF::T>(lengths) >= 2 * mlength_t_min) &&
      (std::get<LF::S>(lengths) >= 2 * mlength_s_min)) {
    //optimize partition for Block_V returning JV, the transformed Block_V, partitionCode_S and arithmetic_model_S
    std::vector<PartitionFlag> partition_mode_v00;
    Block4D transformed_block_V00;
    double JV =
        2.0 * lambda +
        rd_optimize_transform(input_block, transformed_block_V00, position,
            {std::get<LF::T>(lengths) / 2, std::get<LF::S>(lengths) / 2,
                std::get<LF::V>(lengths), std::get<LF::U>(lengths)},
            hierarchical_4d_encoder, lambda, partition_mode_v00);

    if (JV < bestJ) {
      std::vector<PartitionFlag> partition_mode_v01;
      Block4D transformed_block_V01;
      JV += rd_optimize_transform(input_block, transformed_block_V01,
          {std::get<LF::T>(position),
              std::get<LF::S>(position) + std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(position), std::get<LF::U>(position)},
          {std::get<LF::T>(lengths) / 2,
              std::get<LF::S>(lengths) - std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(lengths), std::get<LF::U>(lengths)},
          hierarchical_4d_encoder, lambda, partition_mode_v01);

      if (JV < bestJ) {
        std::vector<PartitionFlag> partition_mode_v11;
        Block4D transformed_block_V11;
        JV += rd_optimize_transform(input_block, transformed_block_V11,
            {std::get<LF::T>(position) + std::get<LF::T>(lengths) / 2,
                std::get<LF::S>(position) + std::get<LF::S>(lengths) / 2,
                std::get<LF::V>(position), std::get<LF::U>(position)},
            {std::get<LF::T>(lengths) - std::get<LF::T>(lengths) / 2,
                std::get<LF::S>(lengths) - std::get<LF::S>(lengths) / 2,
                std::get<LF::V>(lengths), std::get<LF::U>(lengths)},
            hierarchical_4d_encoder, lambda, partition_mode_v11);

        if (JV < bestJ) {
          std::vector<PartitionFlag> partition_mode_v10;
          Block4D transformed_block_V10;
          JV += rd_optimize_transform(input_block, transformed_block_V10,
              {std::get<LF::T>(position) + std::get<LF::T>(lengths) / 2,
                  std::get<LF::S>(position), std::get<LF::V>(position),
                  std::get<LF::U>(position)},
              {std::get<LF::T>(lengths) - std::get<LF::T>(lengths) / 2,
                  std::get<LF::S>(lengths) / 2, std::get<LF::V>(lengths),
                  std::get<LF::U>(lengths)},
              hierarchical_4d_encoder, lambda, partition_mode_v10);

          if (JV < bestJ) {
            bestJ = JV;
            partition_mode = PartitionFlag::viewSplit;
            partition_code_viewSplit.reserve(
                partition_code_viewSplit.size() + partition_mode_v00.size() +
                partition_mode_v01.size() + partition_mode_v11.size() +
                partition_mode_v10.size());
            partition_code_viewSplit.insert(partition_code_viewSplit.end(),
                partition_mode_v00.begin(), partition_mode_v00.end());
            partition_code_viewSplit.insert(partition_code_viewSplit.end(),
                partition_mode_v01.begin(), partition_mode_v01.end());
            partition_code_viewSplit.insert(partition_code_viewSplit.end(),
                partition_mode_v11.begin(), partition_mode_v11.end());
            partition_code_viewSplit.insert(partition_code_viewSplit.end(),
                partition_mode_v10.begin(), partition_mode_v10.end());
            probability_model_for_viewSplit =
                hierarchical_4d_encoder.optimization_probability_models;

            transformed_block_V.set_dimension(lengths);

            transformed_block_V.copy_sub_block_from(transformed_block_V10,
                {0, 0, 0, 0}, {std::get<LF::T>(lengths) / 2, 0, 0, 0});
            transformed_block_V.copy_sub_block_from(transformed_block_V11,
                {0, 0, 0, 0},
                {std::get<LF::T>(lengths) / 2, std::get<LF::S>(lengths) / 2, 0,
                    0});
            transformed_block_V.copy_sub_block_from(transformed_block_V01,
                {0, 0, 0, 0}, {0, std::get<LF::S>(lengths) / 2, 0, 0});
            transformed_block_V.copy_sub_block_from(
                transformed_block_V00, {0, 0, 0, 0});
          }
        }
      }
    }
  }

  partition_code.push_back(partition_mode);

  //inserts the chosen vector of partition flags in the current one
  //copies data from the chosen arithmetic coder model to the current model
  //moves the data from the chosen partition type to the transformed block

  switch (partition_mode) {
    case PartitionFlag::transform: {
      hierarchical_4d_encoder.set_optimization_model(
          probability_model_for_transform);
      transformed_block = std::move(block_0);
      break;
    }
    case PartitionFlag::spatialSplit: {
      partition_code.reserve(
          partition_code.size() + partition_code_spatialSplit.size());
      partition_code.insert(partition_code.end(),
          partition_code_spatialSplit.begin(),
          partition_code_spatialSplit.end());
      hierarchical_4d_encoder.set_optimization_model(
          probability_model_for_spatialSplit);
      transformed_block = std::move(transformed_block_S);
      break;
    }
    case PartitionFlag::viewSplit: {
      partition_code.reserve(
          partition_code.size() + partition_code_viewSplit.size());
      partition_code.insert(partition_code.end(),
          partition_code_viewSplit.begin(), partition_code_viewSplit.end());
      hierarchical_4d_encoder.set_optimization_model(
          probability_model_for_viewSplit);
      transformed_block = std::move(transformed_block_V);
      break;
    }
  }

  return bestJ;
}

void TransformPartition::encode_partition(
    Hierarchical4DEncoder &hierarchical_4d_encoder, double lambda) {
  double scaled_lambda = lambda * hierarchical_4d_encoder.get_number_of_elements_in_transform();

  mPartitionCodeIndex = 0;

  // hierarchical_4d_encoder.encode_integer(hierarchical_4d_encoder.mInferiorBitPlane, MINIMUM_BITPLANE_PRECISION);
  hierarchical_4d_encoder.encode_inferior_bit_plane_value();

  encode_partition({0, 0, 0, 0}, mPartitionData.get_dimension(),
      hierarchical_4d_encoder, scaled_lambda);
}

void TransformPartition::encode_partition(
    const std::tuple<int, int, int, int> &position,
    const std::tuple<int, int, int, int> &lengths,
    Hierarchical4DEncoder &hierarchical_4d_encoder, double lambda) {
  using LF = LightFieldDimension;
  switch (partition_code[mPartitionCodeIndex++]) {
    case PartitionFlag::transform: {
      hierarchical_4d_encoder.encode_partition_transform_flag();
      hierarchical_4d_encoder.mSubbandLF.set_dimension(lengths);
      hierarchical_4d_encoder.mSubbandLF.copy_sub_block_from(
          mPartitionData, position);
      hierarchical_4d_encoder.encode_sub_block(lambda);
      return;
    }
    case PartitionFlag::spatialSplit: {
      hierarchical_4d_encoder.encode_partition_spatialSplit_flag();

      //Encode four spatial subblocks
      encode_partition(position,
          {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
              std::get<LF::V>(lengths) / 2, std::get<LF::U>(lengths) / 2},
          hierarchical_4d_encoder, lambda);
      encode_partition(
          {std::get<LF::T>(position), std::get<LF::S>(position),
              std::get<LF::V>(position),
              std::get<LF::U>(position) + std::get<LF::U>(lengths) / 2},
          {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
              std::get<LF::V>(lengths) / 2,
              std::get<LF::U>(lengths) - std::get<LF::U>(lengths) / 2},
          hierarchical_4d_encoder, lambda);
      encode_partition(
          {std::get<LF::T>(position), std::get<LF::S>(position),
              std::get<LF::V>(position) + std::get<LF::V>(lengths) / 2,
              std::get<LF::U>(position) + std::get<LF::U>(lengths) / 2},
          {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
              std::get<LF::V>(lengths) - std::get<LF::V>(lengths) / 2,
              std::get<LF::U>(lengths) - std::get<LF::U>(lengths) / 2},
          hierarchical_4d_encoder, lambda);
      encode_partition(
          {std::get<LF::T>(position), std::get<LF::S>(position),
              std::get<LF::V>(position) + std::get<LF::V>(lengths) / 2,
              std::get<LF::U>(position)},
          {std::get<LF::T>(lengths), std::get<LF::S>(lengths),
              std::get<LF::V>(lengths) - std::get<LF::V>(lengths) / 2,
              std::get<LF::U>(lengths) / 2},
          hierarchical_4d_encoder, lambda);
      return;
    }
    case PartitionFlag::viewSplit: {
      hierarchical_4d_encoder.encode_partition_viewSplit_flag();

      //Encode four view subblocks
      encode_partition(position,
          {std::get<LF::T>(lengths) / 2, std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(lengths), std::get<LF::U>(lengths)},
          hierarchical_4d_encoder, lambda);
      encode_partition(
          {std::get<LF::T>(position),
              std::get<LF::S>(position) + std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(position), std::get<LF::U>(position)},
          {std::get<LF::T>(lengths) / 2,
              std::get<LF::S>(lengths) - std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(lengths), std::get<LF::U>(lengths)},
          hierarchical_4d_encoder, lambda);
      encode_partition(
          {std::get<LF::T>(position) + std::get<LF::T>(lengths) / 2,
              std::get<LF::S>(position) + std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(position), std::get<LF::U>(position)},
          {std::get<LF::T>(lengths) - std::get<LF::T>(lengths) / 2,
              std::get<LF::S>(lengths) - std::get<LF::S>(lengths) / 2,
              std::get<LF::V>(lengths), std::get<LF::U>(lengths)},
          hierarchical_4d_encoder, lambda);
      encode_partition(
          {std::get<LF::T>(position) + std::get<LF::T>(lengths) / 2,
              std::get<LF::S>(position), std::get<LF::V>(position),
              std::get<LF::U>(position)},
          {std::get<LF::T>(lengths) - std::get<LF::T>(lengths) / 2,
              std::get<LF::S>(lengths) / 2, std::get<LF::V>(lengths),
              std::get<LF::U>(lengths)},
          hierarchical_4d_encoder, lambda);
      return;
    }
  }
}