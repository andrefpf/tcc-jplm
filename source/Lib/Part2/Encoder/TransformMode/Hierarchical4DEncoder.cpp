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

/** \file     Hierarchical4DEncoder.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-26
 */

#include "Hierarchical4DEncoder.h"


ContiguousCodestreamCode& Hierarchical4DEncoder::get_ref_to_codestream_code()
    const {
  return mEntropyCoder.get_ref_to_codestream_code();
}


std::unique_ptr<ContiguousCodestreamCode>&&
Hierarchical4DEncoder::move_codestream_code_out() {
  return mEntropyCoder.move_codestream_code_out();
}


void Hierarchical4DEncoder::reset_probability_models() {
  Hierarchical4DCodec::reset_probability_models();
  optimization_probability_models.reset();
  mEntropyCoder.flush_byte();
}


void Hierarchical4DEncoder ::encode_sub_block(double lambda) {
  hexadecatree_flags.clear();
  auto position = std::make_tuple(0, 0, 0, 0);
  auto lengths = std::make_tuple(mSubbandLF.mlength_t, mSubbandLF.mlength_s,
      mSubbandLF.mlength_v, mSubbandLF.mlength_u);
  rd_optimize_hexadecatree(
      position, lengths, lambda, superior_bit_plane, hexadecatree_flags);
  int flagSearchIndex = 0;
  encode_hexadecatree(0, 0, 0, 0, mSubbandLF.mlength_t, mSubbandLF.mlength_s,
      mSubbandLF.mlength_v, mSubbandLF.mlength_u, superior_bit_plane,
      flagSearchIndex);

  //test if rd_optimize_hexadecatree and encode_hexadecatree gives the same result
}


bool Hierarchical4DEncoder::get_mSubbandLF_significance(uint32_t threshold,
    const LightfieldCoordinate<uint32_t>& position,
    const LightfieldDimension<uint32_t>& range) const {
  const auto elements_to_compute = range - position;

  //addresses to skip before
  const auto ptr_to_skip_before =
      position.hadamard_product(mSubbandLF.get_strides());

  //addresses to skip after
  const auto size_of_remaing_block =
      mSubbandLF.get_dimension() -
      elements_to_compute.hadamard_product({1, 1, 1, 0}) - position;

  const auto ptr_to_skip_after =
      size_of_remaing_block.hadamard_product(mSubbandLF.get_strides());

  auto data_ptr = mSubbandLF.mPixelData + ptr_to_skip_before.get_t();
  for (auto t = 0; t < elements_to_compute.get_t(); ++t) {
    data_ptr += ptr_to_skip_before.get_s();
    for (auto s = 0; s < elements_to_compute.get_s(); ++s) {
      data_ptr += ptr_to_skip_before.get_v();
      for (auto v = 0; v < elements_to_compute.get_v(); ++v) {
        data_ptr += ptr_to_skip_before.get_u();
        auto data_ptr_end = data_ptr + elements_to_compute.get_u();
        auto result = std::find_if(
            data_ptr, data_ptr_end, [threshold](const auto& coefficient) {
              return std::abs(coefficient) >= threshold;
            });
        if (result != data_ptr_end) {
          return true;
        }
        data_ptr += ptr_to_skip_after.get_u();
      }
      data_ptr += ptr_to_skip_after.get_v();
    }
    data_ptr += ptr_to_skip_after.get_s();
  }
  return false;
}


void Hierarchical4DEncoder::create_temporary_buffer() {
  temporary_buffer = std::make_unique<double[]>(this->mTransformLength_u);
}


RDCostResult Hierarchical4DEncoder::get_rd_for_below_inferior_bit_plane(
    const LightfieldCoordinate<uint32_t>& position,
    const LightfieldDimension<uint32_t>& length) {
  double signal_energy = 0.0;
  auto temp = temporary_buffer.get();
  for (auto t = 0; t < length.get_t(); ++t) {
    for (auto s = 0; s < length.get_s(); ++s) {
      for (auto v = 0; v < length.get_v(); ++v) {
        // auto initial_ptr=&mSubbandLF.mPixel[std::get<LF::T>(position)+t][std::get<LF::S>(position)+s][std::get<LF::V>(position)+v][std::get<LF::U>(position)];
        auto initial_ptr =
            mSubbandLF.mPixelData +
            mSubbandLF.get_linear_position(position.get_t() + t,
                position.get_s() + s, position.get_v() + v, position.get_u());
        auto final_ptr = initial_ptr + length.get_u();
        std::transform(initial_ptr, final_ptr, temp, [](int value) -> double {
          return static_cast<double>(value) * static_cast<double>(value);
        });
        signal_energy =
            std::accumulate(temp, temp + length.get_u(), signal_energy);
      }
    }
  }
  //signal energy seems to be the error. As there is no rate (=0), lambda is not important and j is equal error
  return RDCostResult(signal_energy, signal_energy, 0.0, signal_energy);
  //return std::make_pair(signal_energy, signal_energy);
}


RDCostResult Hierarchical4DEncoder::get_rd_for_unitary_block(
    const LightfieldCoordinate<uint32_t>& position, double lambda,
    uint8_t bitplane) {
  const int magnitude = std::abs(mSubbandLF.get_pixel_at(position));

  auto there_is_one = false;
  double accumulated_rate = 0.0;

  int bit_mask = ONES_MASK << inferior_bit_plane;
  int quantized_magnitude = magnitude & bit_mask;

  for (int bit_position = bitplane; bit_position >= inferior_bit_plane;
       --bit_position) {
    int bit = (magnitude >> bit_position) & 01;
    accumulated_rate +=
        optimization_probability_models.get_rate_of_model_and_update(
            bit, bit_position, SYMBOL_PROBABILITY_MODEL_INDEX);
    there_is_one |= bit;
  }

  if (there_is_one) {
    accumulated_rate += 1.0;
    quantized_magnitude += (1 << inferior_bit_plane) / 2;
  }
  double error = magnitude - quantized_magnitude;
  error *= error;  //error squared

  return RDCostResult(error + lambda * accumulated_rate, error,
      accumulated_rate, static_cast<double>(magnitude) * magnitude);
}


RDCostResult Hierarchical4DEncoder::rd_optimize_hexadecatree(
    const std::tuple<int, int, int, int>& position,
    const std::tuple<int, int, int, int>& lengths, double lambda,
    uint8_t bitplane, std::vector<HexadecaTreeFlag>& hexadecatree_flags) {
  auto length = LightfieldDimension<uint32_t>(lengths);
  auto position_coo = LightfieldCoordinate<uint32_t>(position);
  typedef decltype(length)::type lenght_t;
  typedef decltype(position_coo)::type position_t;


  if (bitplane < inferior_bit_plane) {
    return get_rd_for_below_inferior_bit_plane(position_coo, length);
    // return std::make_pair(rd_cost.get_j_cost(), rd_cost.get_energy());
  }


  if (length.has_unitary_area()) {
    return get_rd_for_unitary_block(position_coo, lambda, bitplane);
    // return std::make_pair(rd_cost.get_j_cost(), rd_cost.get_energy());
  }


  decltype(probability_models) currentProbabilityModel =
      optimization_probability_models;


  auto rate_of_skip =
      optimization_probability_models[(bitplane << 1) +
                                      SEGMENTATION_PROB_MODEL_INDEX]
          .get_rate<1>();

  //error and anergy will be obtained latter on
  auto rd_cost_of_skip =
      RDCostResult(lambda * rate_of_skip, 0.0, rate_of_skip, 0.0);
  //j_cost error rate energy

  std::vector<HexadecaTreeFlag> hexadecatree_flags_0;

  auto min_range = position_coo + length;

  const auto should_split_block =
      get_mSubbandLF_significance(1 << bitplane, position_coo, min_range);

  auto segmentation_flags_rate =
      optimization_probability_models.get_segmentation_rate(
          bitplane, should_split_block);

  auto rd_cost_of_segmentation = RDCostResult(
      lambda * segmentation_flags_rate, 0.0, segmentation_flags_rate, 0.0);

  //evaluate the cost J0 to encode this subblock
  if (should_split_block) {  //there was at least one value larger than the threshold (1<<bitplane), it will break the planes by half
    auto half_length = length.divided_by_half_in_all_possible_dimensions();

    //number of divisions in each dimension
    auto n_divisions = length.get_number_of_possible_divisions_by_half();

    for (lenght_t t = 0; t < n_divisions.get_t(); ++t) {
      auto new_position_t = position_coo.get_t() + t * half_length.get_t();
      auto new_length_t = (t == 0) ? half_length.get_t()
                                   : (length.get_t() - half_length.get_t());

      for (lenght_t s = 0; s < n_divisions.get_s(); ++s) {
        auto new_position_s = position_coo.get_s() + s * half_length.get_s();
        auto new_length_s = (s == 0) ? half_length.get_s()
                                     : (length.get_s() - half_length.get_s());

        for (lenght_t v = 0; v < n_divisions.get_v(); ++v) {
          auto new_position_v = position_coo.get_v() + v * half_length.get_v();
          auto new_length_v = (v == 0) ? half_length.get_v()
                                       : (length.get_v() - half_length.get_v());

          for (lenght_t u = 0; u < n_divisions.get_u(); ++u) {
            auto new_position_u =
                position_coo.get_u() + u * half_length.get_u();
            auto new_length_u = (u == 0)
                                    ? half_length.get_u()
                                    : (length.get_u() - half_length.get_u());

            std::vector<HexadecaTreeFlag> hexadecatree_flags_of_partition;

            rd_cost_of_segmentation +=
                rd_optimize_hexadecatree({new_position_t, new_position_s,
                                             new_position_v, new_position_u},
                    {new_length_t, new_length_s, new_length_v, new_length_u},
                    lambda, bitplane, hexadecatree_flags_of_partition);


            hexadecatree_flags_0.reserve(
                hexadecatree_flags_0.size() +
                hexadecatree_flags_of_partition.size());
            hexadecatree_flags_0.insert(hexadecatree_flags_0.end(),
                hexadecatree_flags_of_partition.begin(),
                hexadecatree_flags_of_partition.end());
          }
        }
      }
    }
  } else {  //this means that there is no value larger than the threshold (1<<bitplane). this lower the bitplane
    rd_cost_of_segmentation += rd_optimize_hexadecatree(
        position, lengths, lambda, bitplane - 1, hexadecatree_flags_0);
  }

  rd_cost_of_skip.add_to_energy(rd_cost_of_segmentation.get_energy());
  rd_cost_of_skip.add_to_j_cost(rd_cost_of_segmentation.get_energy());
  rd_cost_of_skip.add_to_error(rd_cost_of_segmentation.get_error());
  //rd_cost_of_skip.add_to_error(rd_cost_of_segmentation.get_energy());

  //Choose the lowest cost
  if ((rd_cost_of_segmentation.get_j_cost() < rd_cost_of_skip.get_j_cost()) ||
      ((bitplane == inferior_bit_plane) && (!should_split_block))) {
    if (should_split_block) {
      hexadecatree_flags.emplace_back(HexadecaTreeFlag::splitBlock);
    } else {
      hexadecatree_flags.emplace_back(HexadecaTreeFlag::lowerBitPlane);
    }
    hexadecatree_flags.reserve(
        hexadecatree_flags.size() + hexadecatree_flags_0.size());
    hexadecatree_flags.insert(hexadecatree_flags.end(),
        hexadecatree_flags_0.begin(), hexadecatree_flags_0.end());
    return rd_cost_of_segmentation;
  }
  //else
  hexadecatree_flags.emplace_back(HexadecaTreeFlag::zeroBlock);

  optimization_probability_models = currentProbabilityModel;

  if (bitplane > BITPLANE_BYPASS_FLAGS)
    optimization_probability_models[(bitplane << 1) +
                                    SEGMENTATION_PROB_MODEL_INDEX]
        .update<1>();

  return rd_cost_of_skip;
}


void Hierarchical4DEncoder::encode_hexadecatree(int position_t, int position_s,
    int position_v, int position_u, int length_t, int length_s, int length_v,
    int length_u, uint8_t bitplane, int& flagIndex) {
  if (bitplane < inferior_bit_plane) {
    return;
  }

  if (length_t * length_s * length_v * length_u == 1) {
    encode_coefficient(
        mSubbandLF.get_pixel_at(position_t, position_s, position_v, position_u),
        bitplane);
    return;
  }

  switch (hexadecatree_flags[flagIndex++]) {
    case HexadecaTreeFlag::lowerBitPlane: {
      encode_segmentation_lowerBitPlane_flag(bitplane);
      encode_hexadecatree(position_t, position_s, position_v, position_u,
          length_t, length_s, length_v, length_u, bitplane - 1, flagIndex);
      return;
    }
    case HexadecaTreeFlag::zeroBlock: {
      encode_segmentation_zeroBlock_flag(bitplane);
      return;
    }
    case HexadecaTreeFlag::splitBlock: {
      encode_segmentation_splitBlock_flag(bitplane);
      int half_length_t = (length_t > 1) ? length_t / 2 : 1;
      int half_length_s = (length_s > 1) ? length_s / 2 : 1;
      int half_length_v = (length_v > 1) ? length_v / 2 : 1;
      int half_length_u = (length_u > 1) ? length_u / 2 : 1;

      int number_of_subdivisions_t = (length_t > 1) ? 2 : 1;
      int number_of_subdivisions_s = (length_s > 1) ? 2 : 1;
      int number_of_subdivisions_v = (length_v > 1) ? 2 : 1;
      int number_of_subdivisions_u = (length_u > 1) ? 2 : 1;

      for (int index_t = 0; index_t < number_of_subdivisions_t; ++index_t) {
        int new_position_t = position_t + index_t * half_length_t;
        int new_length_t =
            (index_t == 0) ? half_length_t : (length_t - half_length_t);

        for (int index_s = 0; index_s < number_of_subdivisions_s; ++index_s) {
          int new_position_s = position_s + index_s * half_length_s;
          int new_length_s =
              (index_s == 0) ? half_length_s : (length_s - half_length_s);

          for (int index_v = 0; index_v < number_of_subdivisions_v; ++index_v) {
            int new_position_v = position_v + index_v * half_length_v;
            int new_length_v =
                (index_v == 0) ? half_length_v : (length_v - half_length_v);

            for (int index_u = 0; index_u < number_of_subdivisions_u;
                 ++index_u) {
              int new_position_u = position_u + index_u * half_length_u;
              int new_length_u =
                  (index_u == 0) ? half_length_u : (length_u - half_length_u);

              encode_hexadecatree(new_position_t, new_position_s,
                  new_position_v, new_position_u, new_length_t, new_length_s,
                  new_length_v, new_length_u, bitplane, flagIndex);
            }
          }
        }
      }
      return;
    }
  }
}


void Hierarchical4DEncoder::encode_coefficient(
    int coefficient, uint8_t bitplane) {
  int sign = 0;
  auto there_is_one = false;
  int magnitude = std::abs(coefficient);
  if (coefficient < 0) {
    sign = 1;
  }
  for (int bit_position = bitplane; bit_position >= inferior_bit_plane;
       bit_position--) {
    int bit = (magnitude >> (bit_position)) & 01;
    mEntropyCoder.encode_bit(
        bit, probability_models[bit_position + SYMBOL_PROBABILITY_MODEL_INDEX]);
    if (bit_position > BITPLANE_BYPASS)
      probability_models[bit_position + SYMBOL_PROBABILITY_MODEL_INDEX].update(
          bit);
    there_is_one |= bit;
  }
  if (there_is_one) {
    mEntropyCoder.encode_bit(sign, probability_models[0]);
  }
}


void Hierarchical4DEncoder::encode_segmentation_lowerBitPlane_flag(
    uint8_t bitplane) {
  auto& probability_model_0 =
      probability_models[(bitplane << 1) + SEGMENTATION_PROB_MODEL_INDEX];
  auto& probability_model_1 =
      probability_models[(bitplane << 1) + SEGMENTATION_PROB_MODEL_INDEX + 1];

  mEntropyCoder.encode_bit<0>(probability_model_0);
  mEntropyCoder.encode_bit<0>(probability_model_1);

  if (bitplane > BITPLANE_BYPASS_FLAGS) {
    probability_model_0.update<0>();
    probability_model_1.update<0>();
  }
}


void Hierarchical4DEncoder::encode_segmentation_splitBlock_flag(
    uint8_t bitplane) {
  auto& probability_model_0 =
      probability_models[(bitplane << 1) + SEGMENTATION_PROB_MODEL_INDEX];
  auto& probability_model_1 =
      probability_models[(bitplane << 1) + SEGMENTATION_PROB_MODEL_INDEX + 1];

  mEntropyCoder.encode_bit<0>(probability_model_0);
  mEntropyCoder.encode_bit<1>(probability_model_1);

  if (bitplane > BITPLANE_BYPASS_FLAGS) {
    probability_model_0.update<0>();
    probability_model_1.update<1>();
  }
}


void Hierarchical4DEncoder::encode_segmentation_zeroBlock_flag(
    uint8_t bitplane) {
  auto& probability_model_0 =
      probability_models[(bitplane << 1) + SEGMENTATION_PROB_MODEL_INDEX];

  mEntropyCoder.encode_bit<1>(probability_model_0);

  if (bitplane > BITPLANE_BYPASS_FLAGS) {
    probability_model_0.update<1>();
  }
}


void Hierarchical4DEncoder::encode_partition_transform_flag() {
  mEntropyCoder.encode_bit<0>(probability_models[0]);
}


void Hierarchical4DEncoder::encode_partition_spatialSplit_flag() {
  const auto& probability_model = probability_models[0];
  mEntropyCoder.encode_bit<1>(probability_model);
  mEntropyCoder.encode_bit<0>(probability_model);
}


void Hierarchical4DEncoder::encode_partition_viewSplit_flag() {
  const auto& probability_model = probability_models[0];
  mEntropyCoder.encode_bit<1>(probability_model);
  mEntropyCoder.encode_bit<1>(probability_model);
}


void Hierarchical4DEncoder::encode_inferior_bit_plane_value() {
  const auto& probability_model = probability_models[0];
  for (int n = MINIMUM_BITPLANE_PRECISION - 1; n >= 0; --n) {
    int bit = (inferior_bit_plane >> n) & 0x01;
    mEntropyCoder.encode_bit(bit, probability_model);
  }
}


int Hierarchical4DEncoder::get_optimum_bit_plane(double lambda) {
  long int subbandSize = mSubbandLF.get_number_of_elements();
  double Jmin = std::numeric_limits<double>::max();  //Irrelevant initial value
  int optimum_bit_plane = 0;  //Irrelevant initial value

  double accumulatedRate = 0.0;

  for (int bit_position = superior_bit_plane; bit_position >= 0;
       bit_position--) {
    double distortion = 0.0;
    auto signalRate = 0;

    int bit_mask = ONES_MASK << bit_position;
    int threshold = (1 << bit_position);

    for (auto data_ptr = mSubbandLF.mPixelData;
         data_ptr < mSubbandLF.mPixelData + subbandSize; ++data_ptr) {
      int magnitude = std::abs(*data_ptr);
      int quantizedMagnitude = magnitude & bit_mask;
      if (quantizedMagnitude > 0) {
        ++signalRate;
        quantizedMagnitude += (threshold >> 1);
      }
      double error = magnitude - quantizedMagnitude;
      distortion += error * error;
      if (magnitude >= threshold) {
        int bit = (magnitude >> bit_position) & 01;
        accumulatedRate +=
            optimization_probability_models[bit_position +
                                            SYMBOL_PROBABILITY_MODEL_INDEX]
                .get_rate(bit);
        optimization_probability_models[bit_position +
                                        SYMBOL_PROBABILITY_MODEL_INDEX]
            .update(bit);
      }
    }

    //since accumulateRate is monotonically increasing, if lambda*accumulatedRate is > Jmin, no other case will be smaller...
    if (lambda * (accumulatedRate) >
        Jmin) {  //<! \todo check if this condition happens
      return optimum_bit_plane;
    }

    double J = distortion +
               lambda * (accumulatedRate + static_cast<double>(signalRate));

    if (J <= Jmin) {
      Jmin = J;
      optimum_bit_plane = bit_position;
    }
  }

  return optimum_bit_plane;
  //after this opertation, the optimizer prob state is reset
}


void Hierarchical4DEncoder::set_optimization_model(
    const ProbabilityModelsHandler& model) {
  optimization_probability_models = model;
}


void Hierarchical4DEncoder::load_optimizer_state() {
  optimization_probability_models = probability_models;
}
