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

/** \file     JPLM4DTransformModeLightFieldCodec.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-10-09
 */

#ifndef JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__
#define JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__

#include <cstdint>
#include <iostream>
#include "Lib/Part2/Common/JPLMLightFieldCodec.h"
#include "Lib/Part2/Common/TransformMode/DCT4DCoefficientsManager.h"
#include "Lib/Part2/Common/TransformMode/LightFieldTransformMode.h"
#include "Lib/Utils/Image/ColorSpaces.h"

template<typename PelType = uint16_t>
class JPLM4DTransformModeLightFieldCodec
    : public virtual JPLMLightFieldCodec<PelType> {
 protected:
  bool needs_block_extension = false;
  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> extensions;
  LightfieldDimension<uint32_t> lightfield_dimension;
  LightfieldDimension<uint32_t> block_4d_dimension;

 public:
  JPLM4DTransformModeLightFieldCodec(
      const LightfieldDimension<uint32_t>& lightfield_dimension,
      const LightfieldDimension<uint32_t>& block_4d_dimension)
      : JPLMLightFieldCodec<PelType>(nullptr),
        lightfield_dimension(lightfield_dimension),
        block_4d_dimension(block_4d_dimension) {
  }


  virtual ~JPLM4DTransformModeLightFieldCodec() = default;


  void setup_transform_coefficients(bool forward,
      const std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>& max_sizes,
      const std::tuple<double, double, double, double>& scalings) {
    const auto& [t_size, s_size, v_size, u_size] = max_sizes;
    const auto& [t_scaling, s_scaling, v_scaling, u_scaling] = scalings;
    DCT4DCoefficientsManager::get_instance(forward).set_transform_max_sizes(
        u_size, v_size, s_size, t_size);

    DCT4DCoefficientsManager::get_instance(forward).set_transform_gains(
        u_scaling, v_scaling, s_scaling, t_scaling);
  }


  void initialize_extension_lenghts() {
    // extension_length_t = parameter_handler.number_of_vertical_views%parameter_handler.transform_length_t;
    // extension_length_s = parameter_handler.number_of_horizontal_views%parameter_handler.transform_length_s;
    // extension_length_v = decoded_lightfield.mNumberOfViewLines%parameter_handler.transform_length_v;
    // extension_length_u = decoded_lightfield.mNumberOfViewColumns%parameter_handler.transform_length_u;

    // if(parameter_handler.extension_method != SHRINK_TO_FIT && extension_length_t+extension_length_s+extension_length_v+extension_length_u > 0)
    //     needs_block_extension=true;
  }


  virtual void finalization() {};

  virtual void run() override {
    const auto& [T, S, V, U] = lightfield_dimension;

    const auto& [BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u] =
        block_4d_dimension;


    int32_t level_shift = 512;
    //std::pow(2, lightfield->get_views_bpp()) / 2;  //

    for (auto t = decltype(T){0}; t < T; t += BLOCK_SIZE_t) {
      auto used_size_t =
          (t + BLOCK_SIZE_t > T) ? T % BLOCK_SIZE_t : BLOCK_SIZE_t;
      for (auto s = decltype(S){0}; s < S; s += BLOCK_SIZE_s) {
        auto used_size_s =
            (s + BLOCK_SIZE_s > S) ? S % BLOCK_SIZE_s : BLOCK_SIZE_s;
        for (auto v = decltype(V){0}; v < V; v += BLOCK_SIZE_v) {
          auto used_size_v =
              (v + BLOCK_SIZE_v > V) ? V % BLOCK_SIZE_v : BLOCK_SIZE_v;
          for (auto u = decltype(U){0}; u < U; u += BLOCK_SIZE_u) {
            auto used_size_u =
                (u + BLOCK_SIZE_u > U) ? U % BLOCK_SIZE_u : BLOCK_SIZE_u;


            // if (parameter_handler.verbose) {
            printf("transforming the 4D block at position (%d %d %d %d)\n", t,
                s, v, u);
            // }

            //if (parameter_handler.extension_method == SHRINK_TO_FIT) {
            //    rgb_4d_block.resize_blocks(used_size_t, used_size_s, used_size_v, used_size_u);
            //    spectral_4d_block.resize_blocks(used_size_t, used_size_s, used_size_v, used_size_u);
            //}


            auto size = LightfieldDimension<uint32_t>(
                used_size_t, used_size_s, used_size_v, used_size_u);

            for (auto color_channel_index = 0; color_channel_index < 3;
                 ++color_channel_index) {
              run_for_block_4d(
                  color_channel_index, level_shift, {t, s, v, u}, size);
            }
          }
        }
      }
    }
    //up to now, this finalization step is required only in the encoder.
    finalization();
  }


  virtual void run_for_block_4d(const uint32_t channel,
      const int32_t level_shift, const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& size) = 0;
};


#endif /* end of include guard: JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__ */