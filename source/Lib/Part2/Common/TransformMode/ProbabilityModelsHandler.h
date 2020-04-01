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

/** \file     ProbabilityModelsHandler.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-03
 */

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_PROBABILITY_MODELS_HANDLER_H
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_PROBABILITY_MODELS_HANDLER_H

#include <array>
#include "Lib/Part2/Common/TransformMode/ProbabilityModel.h"

class ProbabilityModelsHandler {
 protected:
  // static constexpr std::size_t BITPLANE_BYPASS = 0;
  // static constexpr std::size_t BITPLANE_BYPASS_FLAGS = 0;
  static constexpr auto SEGMENTATION_PROB_MODEL_INDEX = 32;
  static constexpr auto number_of_probability_models = 161;
  std::array<ProbabilityModel, number_of_probability_models>
      probability_models_array;

 public:
  ProbabilityModelsHandler() = default;
  virtual ~ProbabilityModelsHandler() = default;

  void reset() {
    for (auto& probability_model : probability_models_array) {
      probability_model.reset();
    }
  }


  template<bool bit>
  double get_rate_of_model_and_update(
      std::size_t bit_position, std::size_t stride) {
    auto rate = probability_models_array[bit_position + stride].get_rate<bit>();
    // \todo: remove always true if (bit_position >= BITPLANE_BYPASS) {
      probability_models_array[bit_position + stride].update<bit>();
    // }
    return rate;
  }


  double get_rate_of_model_and_update(
      bool bit, std::size_t bit_position, std::size_t stride) {
    if (bit) {
      return get_rate_of_model_and_update<1>(bit_position, stride);
    }
    return get_rate_of_model_and_update<0>(bit_position, stride);
  }


  double get_segmentation_rate(std::size_t bitplane, bool significance) {
    const auto position = (bitplane << 1) + SEGMENTATION_PROB_MODEL_INDEX;
    auto rate = probability_models_array[position].get_rate<0>() +
                probability_models_array[position + 1].get_rate(significance);
    // \todo: remove always true if (bitplane >= BITPLANE_BYPASS_FLAGS) {
      probability_models_array[position].update<0>();
      probability_models_array[position + 1].update(significance);
    // }
    return rate;
  }

  ProbabilityModel& operator[](std::size_t index) {
    return probability_models_array[index];
  }
};

#endif  // JPLM_LIB_PART2_COMMON_TRANSFORMMODE_PROBABILITY_MODELS_HANDLER_H