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


void Hierarchical4DEncoder::start(FILE* outputFilePointer) {
  mEntropyCoder.start(outputFilePointer);  //opens output file
  reset_probability_models();
}

void Hierarchical4DEncoder::reset_optimization_models() {
  for (auto& probability_model : optimization_probability_models) {
    probability_model.reset();
  }
}

void Hierarchical4DEncoder::reset_probability_models() {
  Hierarchical4DCodec::reset_probability_models();
  reset_optimization_models();
}

void Hierarchical4DEncoder ::EncodeSubblock(double lambda) {
  hexadecatree_flags.clear();
  auto position = std::make_tuple(0, 0, 0, 0);
  auto lengths = std::make_tuple(mSubbandLF.mlength_t, mSubbandLF.mlength_s,
      mSubbandLF.mlength_v, mSubbandLF.mlength_u);
  RdOptimizeHexadecaTree(
      position, lengths, lambda, mSuperiorBitPlane, hexadecatree_flags);
  int flagSearchIndex = 0;
  encode_hexadecatree(0, 0, 0, 0, mSubbandLF.mlength_t, mSubbandLF.mlength_s,
      mSubbandLF.mlength_v, mSubbandLF.mlength_u, mSuperiorBitPlane,
      flagSearchIndex);
}

bool Hierarchical4DEncoder::get_mSubbandLF_significance(int bitplane,
    const std::tuple<int, int, int, int>& position,
    const std::tuple<int, int, int, int>& range) const {
  using LF = LightFieldDimension;
  auto threshold = 1 << bitplane;

  auto elements_to_compute_in_t =
      std::get<LF::T>(range) - std::get<LF::T>(position);
  auto elements_to_skip_before_in_t =
      std::get<LF::T>(position) * mSubbandLF.stride_t;

  auto elements_to_compute_in_s =
      std::get<LF::S>(range) - std::get<LF::S>(position);
  auto elements_to_skip_before_in_s =
      std::get<LF::S>(position) * mSubbandLF.stride_s;
  auto elements_to_skip_after_in_s =
      (mSubbandLF.mlength_s - elements_to_compute_in_s -
          std::get<LF::S>(position)) *
      mSubbandLF.stride_s;

  auto elements_to_compute_in_v =
      std::get<LF::V>(range) - std::get<LF::V>(position);
  auto elements_to_skip_before_in_v =
      std::get<LF::V>(position) * mSubbandLF.stride_v;
  auto elements_to_skip_after_in_v =
      (mSubbandLF.mlength_v - elements_to_compute_in_v -
          std::get<LF::V>(position)) *
      mSubbandLF.stride_v;

  auto elements_to_compute_in_u =
      std::get<LF::U>(range) - std::get<LF::U>(position);
  auto elements_to_skip_before_in_u = std::get<LF::U>(position);
  auto elements_to_skip_after_in_u = mSubbandLF.mlength_u -
                                     elements_to_compute_in_u -
                                     std::get<LF::U>(position);

  block4DElementType coefficient;

  auto data_ptr = mSubbandLF.mPixelData + elements_to_skip_before_in_t;
  for (auto t = 0; t < elements_to_compute_in_t; t++) {
    data_ptr += elements_to_skip_before_in_s;
    for (auto s = 0; s < elements_to_compute_in_s; s++) {
      data_ptr += elements_to_skip_before_in_v;
      for (auto v = 0; v < elements_to_compute_in_v; v++) {
        data_ptr += elements_to_skip_before_in_u;
        for (auto u = 0; u < elements_to_compute_in_u; u++) {
          coefficient = std::abs(*data_ptr);
          if (coefficient >= threshold)
            return true;
          data_ptr++;
        }
        data_ptr += elements_to_skip_after_in_u;
      }
      data_ptr += elements_to_skip_after_in_v;
    }
    data_ptr += elements_to_skip_after_in_s;
  }
  return false;
}

void Hierarchical4DEncoder::create_temporary_buffer(int size) {
  temporary_buffer = std::make_unique<double[]>(size);
}

std::pair<double, double> Hierarchical4DEncoder::RdOptimizeHexadecaTree(
    const std::tuple<int, int, int, int>& position,
    const std::tuple<int, int, int, int>& lengths, double lambda, int bitplane,
    std::vector<HexadecaTreeFlag>& hexadecatree_flags) {
  using LF = LightFieldDimension;

  if (bitplane < mInferiorBitPlane) {
    double signal_energy = 0.0;
    auto temp = temporary_buffer.get();
    for (auto t = 0; t < std::get<LF::T>(lengths); t++) {
      for (auto s = 0; s < std::get<LF::S>(lengths); s++) {
        for (auto v = 0; v < std::get<LF::V>(lengths); v++) {
          // auto initial_ptr=&mSubbandLF.mPixel[std::get<LF::T>(position)+t][std::get<LF::S>(position)+s][std::get<LF::V>(position)+v][std::get<LF::U>(position)];
          auto initial_ptr =
              mSubbandLF.mPixelData +
              mSubbandLF.get_linear_position(std::get<LF::T>(position) + t,
                  std::get<LF::S>(position) + s, std::get<LF::V>(position) + v,
                  std::get<LF::U>(position));
          auto final_ptr = initial_ptr + std::get<LF::U>(lengths);
          std::transform(initial_ptr, final_ptr, temp, [](int value) -> double {
            return static_cast<double>(value) * static_cast<double>(value);
          });
          signal_energy = std::accumulate(
              temp, temp + std::get<LF::U>(lengths), signal_energy);
        }
      }
    }
    return std::make_pair(signal_energy, signal_energy);
  }

  if (std::get<LF::T>(lengths) * std::get<LF::S>(lengths) *
          std::get<LF::V>(lengths) * std::get<LF::U>(lengths) ==
      1) {
    const int magnitude = std::abs(mSubbandLF.get_pixel_at(
        std::get<LF::T>(position), std::get<LF::S>(position),
        std::get<LF::V>(position), std::get<LF::U>(position)));

    auto there_is_one = false;
    double accumulatedRate = 0.0;
    for (int bit_position = bitplane; bit_position >= mInferiorBitPlane;
         bit_position--) {
      int bit = (magnitude >> bit_position) & 01;
      if (bit) {
        there_is_one = true;
        accumulatedRate +=
            optimization_probability_models[bit_position +
                                            mSymbolProbabilityModelIndex]
                .get_rate<1>();
        if (bit_position > BITPLANE_BYPASS)
          optimization_probability_models[bit_position +
                                          mSymbolProbabilityModelIndex]
              .update<1>();
      } else {
        accumulatedRate +=
            optimization_probability_models[bit_position +
                                            mSymbolProbabilityModelIndex]
                .get_rate<0>();
        if (bit_position > BITPLANE_BYPASS)
          optimization_probability_models[bit_position +
                                          mSymbolProbabilityModelIndex]
              .update<0>();
      }
    }
    if (there_is_one)
      accumulatedRate += 1.0;

    int bitMask = onesMask << mInferiorBitPlane;
    int quantizedMagnitude = magnitude & bitMask;
    if (there_is_one) {
      quantizedMagnitude += (1 << mInferiorBitPlane) / 2;
    }
    double error = magnitude - quantizedMagnitude;

    return std::make_pair(error * error + lambda * accumulatedRate,
        static_cast<double>(magnitude) * magnitude);
  }

  decltype(probability_models) currentProbabilityModel =
      optimization_probability_models;

  std::vector<HexadecaTreeFlag> hexadecatree_flags_0;

  auto min_range_t =
      std::min(std::get<LF::T>(position) + std::get<LF::T>(lengths),
          mSubbandLF.mlength_t);
  auto min_range_s =
      std::min(std::get<LF::S>(position) + std::get<LF::S>(lengths),
          mSubbandLF.mlength_s);
  auto min_range_v =
      std::min(std::get<LF::V>(position) + std::get<LF::V>(lengths),
          mSubbandLF.mlength_v);
  auto min_range_u =
      std::min(std::get<LF::U>(position) + std::get<LF::U>(lengths),
          mSubbandLF.mlength_u);

  const auto Significance = get_mSubbandLF_significance(
      bitplane, position, {min_range_t, min_range_s, min_range_v, min_range_u});


  auto segmentation_flags_j_cost =
      lambda * optimization_probability_models
                   [(bitplane << 1) + mSegmentationFlagProbabilityModelIndex]
                       .get_rate<0>() +
      lambda *
          optimization_probability_models
              [(bitplane << 1) + 1 + mSegmentationFlagProbabilityModelIndex]
                  .get_rate(Significance);

  std::pair<double, double> J_and_energy =
      std::make_pair(segmentation_flags_j_cost, 0.0);
  auto j_skip =
      lambda *
      optimization_probability_models[(bitplane << 1) +
                                      mSegmentationFlagProbabilityModelIndex]
          .get_rate<1>();

  if (bitplane > BITPLANE_BYPASS_FLAGS) {
    optimization_probability_models[(bitplane << 1) +
                                    mSegmentationFlagProbabilityModelIndex]
        .update<0>();
    optimization_probability_models[(bitplane << 1) + 1 +
                                    mSegmentationFlagProbabilityModelIndex]
        .update(Significance);
  }

  //evaluate the cost J0 to encode this subblock
  if (!Significance) {  //this means that there is no value larger than the threshold (1<<bitplane)
    auto temp_j_and_energy = RdOptimizeHexadecaTree(
        position, lengths, lambda, bitplane - 1, hexadecatree_flags_0);
    std::get<0>(J_and_energy) += std::get<0>(temp_j_and_energy);
    std::get<1>(J_and_energy) += std::get<1>(temp_j_and_energy);
  } else {  //there was at least one value larger than the threshold (1<<bitplane), it will break the planes by half
    std::tuple<int, int, int, int> half_lenghts = std::experimental::apply(
        [](auto... x) { return std::make_tuple(x > 1 ? x / 2 : 1 ...); },
        lengths);
    auto number_of_subdivisions = std::experimental::apply(
        [](auto... x) { return std::make_tuple(x > 1 ? 2 : 1 ...); }, lengths);

    for (int t = 0; t < std::get<LF::T>(number_of_subdivisions); t++) {
      int new_position_t =
          std::get<LF::T>(position) + t * std::get<LF::T>(half_lenghts);
      int new_length_t =
          (t == 0) ? std::get<LF::T>(half_lenghts)
                   : (std::get<LF::T>(lengths) - std::get<LF::T>(half_lenghts));

      for (int s = 0; s < std::get<LF::S>(number_of_subdivisions); s++) {
        int new_position_s =
            std::get<LF::S>(position) + s * std::get<LF::S>(half_lenghts);
        int new_length_s =
            (s == 0)
                ? std::get<LF::S>(half_lenghts)
                : (std::get<LF::S>(lengths) - std::get<LF::S>(half_lenghts));

        for (int v = 0; v < std::get<LF::V>(number_of_subdivisions); v++) {
          int new_position_v =
              std::get<LF::V>(position) + v * std::get<LF::V>(half_lenghts);
          int new_length_v =
              (v == 0)
                  ? std::get<LF::V>(half_lenghts)
                  : (std::get<LF::V>(lengths) - std::get<LF::V>(half_lenghts));

          for (int u = 0; u < std::get<LF::U>(number_of_subdivisions); u++) {
            int new_position_u =
                std::get<LF::U>(position) + u * std::get<LF::U>(half_lenghts);
            int new_length_u = (u == 0) ? std::get<LF::U>(half_lenghts)
                                        : (std::get<LF::U>(lengths) -
                                              std::get<LF::U>(half_lenghts));

            std::vector<HexadecaTreeFlag> hexadecatree_flags_1;

            auto temp_j_and_energy =
                RdOptimizeHexadecaTree({new_position_t, new_position_s,
                                           new_position_v, new_position_u},
                    {new_length_t, new_length_s, new_length_v, new_length_u},
                    lambda, bitplane, hexadecatree_flags_1);
            std::get<0>(J_and_energy) += std::get<0>(temp_j_and_energy);
            std::get<1>(J_and_energy) += std::get<1>(temp_j_and_energy);

            hexadecatree_flags_0.reserve(
                hexadecatree_flags_0.size() + hexadecatree_flags_1.size());
            hexadecatree_flags_0.insert(hexadecatree_flags_0.end(),
                hexadecatree_flags_1.begin(), hexadecatree_flags_1.end());
          }
        }
      }
    }
  }

  double J0 = std::get<0>(J_and_energy);
  double SignalEnergySum = std::get<1>(J_and_energy);

  j_skip += SignalEnergySum;

  //Choose the lowest cost
  if ((J0 < j_skip) || ((bitplane == mInferiorBitPlane) && (!Significance))) {
    std::vector<HexadecaTreeFlag> temp_hexadecatree_flags = hexadecatree_flags;

    if (Significance) {
      hexadecatree_flags.emplace_back(HexadecaTreeFlag::splitBlock);
    } else {
      hexadecatree_flags.emplace_back(HexadecaTreeFlag::lowerBitPlane);
    }
    hexadecatree_flags.reserve(
        hexadecatree_flags.size() + hexadecatree_flags_0.size());
    hexadecatree_flags.insert(hexadecatree_flags.end(),
        hexadecatree_flags_0.begin(), hexadecatree_flags_0.end());
  } else {
    hexadecatree_flags.emplace_back(HexadecaTreeFlag::zeroBlock);
    J0 = j_skip;

    optimization_probability_models = currentProbabilityModel;

    if (bitplane > BITPLANE_BYPASS_FLAGS)
      optimization_probability_models[(bitplane << 1) +
                                      mSegmentationFlagProbabilityModelIndex]
          .update<1>();
  }

  std::get<0>(J_and_energy) = J0;

  return J_and_energy;
}


void Hierarchical4DEncoder::encode_hexadecatree(int position_t, int position_s,
    int position_v, int position_u, int length_t, int length_s, int length_v,
    int length_u, int bitplane, int& flagIndex) {
  if (bitplane < mInferiorBitPlane) {
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

      for (int index_t = 0; index_t < number_of_subdivisions_t; index_t++) {
        int new_position_t = position_t + index_t * half_length_t;
        int new_length_t =
            (index_t == 0) ? half_length_t : (length_t - half_length_t);

        for (int index_s = 0; index_s < number_of_subdivisions_s; index_s++) {
          int new_position_s = position_s + index_s * half_length_s;
          int new_length_s =
              (index_s == 0) ? half_length_s : (length_s - half_length_s);

          for (int index_v = 0; index_v < number_of_subdivisions_v; index_v++) {
            int new_position_v = position_v + index_v * half_length_v;
            int new_length_v =
                (index_v == 0) ? half_length_v : (length_v - half_length_v);

            for (int index_u = 0; index_u < number_of_subdivisions_u;
                 index_u++) {
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

void Hierarchical4DEncoder::encode_coefficient(int coefficient, int bitplane) {
  int sign = 0;
  auto there_is_one = false;
  int magnitude = std::abs(coefficient);
  if (coefficient < 0) {
    sign = 1;
  }
  for (int bit_position = bitplane; bit_position >= mInferiorBitPlane;
       bit_position--) {
    int bit = (magnitude >> (bit_position)) & 01;
    mEntropyCoder.encode_bit(
        bit, probability_models[bit_position + mSymbolProbabilityModelIndex]);
    if (bit_position > BITPLANE_BYPASS)
      probability_models[bit_position + mSymbolProbabilityModelIndex].update(
          bit);
    there_is_one |= bit;
  }
  if (there_is_one) {
    mEntropyCoder.encode_bit(sign, probability_models[0]);
  }
}

void Hierarchical4DEncoder::encode_segmentation_lowerBitPlane_flag(
    int bitplane) {
  mEntropyCoder.encode_bit(
      0, probability_models[(bitplane << 1) +
                            mSegmentationFlagProbabilityModelIndex]);
  mEntropyCoder.encode_bit(
      0, probability_models[(bitplane << 1) + 1 +
                            mSegmentationFlagProbabilityModelIndex]);
  if (bitplane > BITPLANE_BYPASS_FLAGS) {
    probability_models[(bitplane << 1) + mSegmentationFlagProbabilityModelIndex]
        .update<0>();
    probability_models[(bitplane << 1) + 1 +
                       mSegmentationFlagProbabilityModelIndex]
        .update<0>();
  }
}

void Hierarchical4DEncoder::encode_segmentation_splitBlock_flag(int bitplane) {
  mEntropyCoder.encode_bit(
      0, probability_models[(bitplane << 1) +
                            mSegmentationFlagProbabilityModelIndex]);
  mEntropyCoder.encode_bit(
      1, probability_models[(bitplane << 1) + 1 +
                            mSegmentationFlagProbabilityModelIndex]);
  if (bitplane > BITPLANE_BYPASS_FLAGS) {
    probability_models[(bitplane << 1) + mSegmentationFlagProbabilityModelIndex]
        .update<0>();
    probability_models[(bitplane << 1) + 1 +
                       mSegmentationFlagProbabilityModelIndex]
        .update<1>();
  }
}

void Hierarchical4DEncoder::encode_segmentation_zeroBlock_flag(int bitplane) {
  mEntropyCoder.encode_bit(
      1, probability_models[(bitplane << 1) +
                            mSegmentationFlagProbabilityModelIndex]);
  if (bitplane > BITPLANE_BYPASS_FLAGS)
    probability_models[(bitplane << 1) + mSegmentationFlagProbabilityModelIndex]
        .update<1>();
}

void Hierarchical4DEncoder::encode_partition_transform_flag() {
  mEntropyCoder.encode_bit(0, probability_models[0]);
}

void Hierarchical4DEncoder::encode_partition_spatialSplit_flag() {
  mEntropyCoder.encode_bit(1, probability_models[0]);
  mEntropyCoder.encode_bit(0, probability_models[0]);
}

void Hierarchical4DEncoder::encode_partition_viewSplit_flag() {
  mEntropyCoder.encode_bit(1, probability_models[0]);
  mEntropyCoder.encode_bit(1, probability_models[0]);
}

void Hierarchical4DEncoder::encode_inferior_bit_plane_value() {
  for (int n = MINIMUM_BITPLANE_PRECISION - 1; n >= 0; n--) {
    int bit = (mInferiorBitPlane >> n) & 01;
    mEntropyCoder.encode_bit(bit, probability_models[0]);
  }
}

int Hierarchical4DEncoder::get_optimum_bit_plane(double lambda) {
  long int subbandSize = mSubbandLF.get_number_of_elements();
  double Jmin = std::numeric_limits<double>::max();  //Irrelevant initial value
  int optimum_bit_plane = 0;  //Irrelevant initial value

  double accumulatedRate = 0.0;
  int onesMask = 0;
  onesMask = ~onesMask;

  for (int bit_position = mSuperiorBitPlane; bit_position >= 0;
       bit_position--) {
    double distortion = 0.0;
    auto signalRate = 0;

    int bitMask = onesMask << bit_position;
    int threshold = (1 << bit_position);

    for (auto data_ptr = mSubbandLF.mPixelData;
         data_ptr < mSubbandLF.mPixelData + subbandSize; data_ptr++) {
      int magnitude = std::abs(*data_ptr);
      int quantizedMagnitude = magnitude & bitMask;
      if (quantizedMagnitude > 0) {
        signalRate++;
        quantizedMagnitude += (threshold >> 1);
      }
      double error = magnitude - quantizedMagnitude;
      distortion += error * error;
      if (magnitude >= threshold) {
        int bit = (magnitude >> bit_position) & 01;
        accumulatedRate +=
            optimization_probability_models[bit_position +
                                            mSymbolProbabilityModelIndex]
                .get_rate(bit);
        optimization_probability_models[bit_position +
                                        mSymbolProbabilityModelIndex]
            .update(bit);
      }
    }

    //since accumulateRate is monotonically increasing, if lambda*accumulatedRate is > Jmin, no other case will be smaller...
    if (lambda * (accumulatedRate) > Jmin)
      return optimum_bit_plane;

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

void Hierarchical4DEncoder::set_optimization_model(std::array<ProbabilityModel,
    Hierarchical4DEncoder::number_of_probability_models>& model) {
  optimization_probability_models = model;
}

void Hierarchical4DEncoder::load_optimizer_state() {
  optimization_probability_models = probability_models;
}
