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
#include "Lib/Part2/Common/TransformMode/BorderBlocksPolicy.h"
#include "Lib/Part2/Common/TransformMode/DCT4DCoefficientsManager.h"
#include "Lib/Part2/Common/TransformMode/LightFieldTransformMode.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "cppitertools/product.hpp"
#include "cppitertools/zip.hpp"


template<typename PelType = uint16_t>
class JPLM4DTransformModeLightFieldCodec
    : public virtual JPLMLightFieldCodec<PelType> {
 protected:
  bool needs_block_extension = false;
  uint32_t number_of_colour_components = 3;
  std::tuple<uint32_t, uint32_t, uint32_t, uint32_t> extensions;
  std::tuple<bool, bool, bool, bool> extends_in_direction = {
      false, false, false, false};
  LightfieldDimension<uint32_t> lightfield_dimension;
  LightfieldDimension<uint32_t> block_4d_dimension;
  const JPLMConfiguration& transform_mode_configuration;

  virtual uint16_t get_number_of_colour_components() const = 0;

  std::vector<uint32_t> get_vector_of_positions_in_dimension(
      uint32_t total_size, uint32_t block_size) const {
    auto number_of_positions = static_cast<int>(
        std::ceil(total_size / static_cast<double>(block_size)));
    std::vector<uint32_t> positions(number_of_positions);
    positions.at(0) = 0;
    auto value = 0;
    std::generate(positions.begin() + 1, positions.end(),
        [&value, block_size] { return value += block_size; });
    return positions;
  }


  auto get_block_coordinates_and_sizes() const {
    const auto& [T, S, V, U] = lightfield_dimension;
    const auto& [BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u] =
        block_4d_dimension;

    auto t_coordinates = get_vector_of_positions_in_dimension(T, BLOCK_SIZE_t);
    auto s_coordinates = get_vector_of_positions_in_dimension(S, BLOCK_SIZE_s);
    auto v_coordinates = get_vector_of_positions_in_dimension(V, BLOCK_SIZE_v);
    auto u_coordinates = get_vector_of_positions_in_dimension(U, BLOCK_SIZE_u);


    auto t_sizes = std::vector<uint32_t>(t_coordinates.size(), BLOCK_SIZE_t);
    auto s_sizes = std::vector<uint32_t>(s_coordinates.size(), BLOCK_SIZE_s);
    auto v_sizes = std::vector<uint32_t>(v_coordinates.size(), BLOCK_SIZE_v);
    auto u_sizes = std::vector<uint32_t>(u_coordinates.size(), BLOCK_SIZE_u);


    const auto& boder_blocks_policy = this->get_border_blocks_policy();
    if (boder_blocks_policy == BorderBlocksPolicy::truncate) {
      if (transform_mode_configuration.is_verbose()) {
        std::cout << "Border policy: truncate";
      }
      if (std::get<0>(extends_in_direction)) {
        t_sizes.back() = T % BLOCK_SIZE_t;
      }
      if (std::get<1>(extends_in_direction)) {
        s_sizes.back() = S % BLOCK_SIZE_s;
      }
      if (std::get<2>(extends_in_direction)) {
        v_sizes.back() = V % BLOCK_SIZE_v;
      }
      if (std::get<3>(extends_in_direction)) {
        u_sizes.back() = U % BLOCK_SIZE_u;
      }
    }

    auto&& coordinates_iter = iter::product(
        t_coordinates, s_coordinates, v_coordinates, u_coordinates);

    auto&& sizes_iter = iter::product(t_sizes, s_sizes, v_sizes, u_sizes);

    auto coordinates_vector =
        std::vector<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>>(
            coordinates_iter.begin(), coordinates_iter.end());
    auto sizes_vector =
        std::vector<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>>(
            sizes_iter.begin(), sizes_iter.end());

    auto&& zip_iter = iter::zip(coordinates_vector, sizes_vector);

    std::vector<std::tuple<std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>,
        std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>>>
        ziped_vector(zip_iter.begin(), zip_iter.end());

    return ziped_vector;
  }


 public:
  JPLM4DTransformModeLightFieldCodec(
      const LightfieldDimension<uint32_t>& lightfield_dimension,
      const LightfieldDimension<uint32_t>& block_4d_dimension,
      const JPLMConfiguration& configuration)
      : JPLMLightFieldCodec<PelType>(nullptr),
        lightfield_dimension(lightfield_dimension),
        block_4d_dimension(block_4d_dimension),
        transform_mode_configuration(configuration) {
  }


  virtual ~JPLM4DTransformModeLightFieldCodec() = default;


  void setup_transform_coefficients(bool forward,
      const std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>& max_sizes,
      const std::tuple<double, double, double, double>& scalings);


  void initialize_extension_lengths();


  virtual void finalization(){};


  virtual BorderBlocksPolicy get_border_blocks_policy() const = 0;


  virtual void run() override;


  virtual void run_for_block_4d(const uint32_t channel,
      const LightfieldCoordinate<uint32_t>& position,
      const LightfieldDimension<uint32_t>& size) = 0;
};


template<typename PelType>
void JPLM4DTransformModeLightFieldCodec<PelType>::setup_transform_coefficients(
    bool forward,
    const std::tuple<uint32_t, uint32_t, uint32_t, uint32_t>& max_sizes,
    const std::tuple<double, double, double, double>& scalings) {
  const auto& [t_size, s_size, v_size, u_size] = max_sizes;
  const auto& [t_scaling, s_scaling, v_scaling, u_scaling] = scalings;
  DCT4DCoefficientsManager::get_instance(forward).set_transform_max_sizes(
      u_size, v_size, s_size, t_size);

  DCT4DCoefficientsManager::get_instance(forward).set_transform_gains(
      u_scaling, v_scaling, s_scaling, t_scaling);
}


template<typename PelType>
void JPLM4DTransformModeLightFieldCodec<
    PelType>::initialize_extension_lengths() {
  if (transform_mode_configuration.is_verbose()) {
    std::cout << "Initializing extension lengths" << std::endl;
  }

  const auto& [number_of_vertical_views, number_of_horizontal_views,
                  mNumberOfViewLines, mNumberOfViewColumns] =
      lightfield_dimension.as_tuple();
  const auto& [transform_length_t, transform_length_s, transform_length_v,
                  transform_length_u] = block_4d_dimension.as_tuple();


  auto extension_length_t = number_of_vertical_views % transform_length_t;
  auto extension_length_s = number_of_horizontal_views % transform_length_s;
  auto extension_length_v = mNumberOfViewLines % transform_length_v;
  auto extension_length_u = mNumberOfViewColumns % transform_length_u;

  if (extension_length_t != 0) {
    needs_block_extension = true;
    std::get<0>(extends_in_direction) = true;
  }
  if (extension_length_s != 0) {
    needs_block_extension = true;
    std::get<1>(extends_in_direction) = true;
  }
  if (extension_length_v != 0) {
    needs_block_extension = true;
    std::get<2>(extends_in_direction) = true;
  }
  if (extension_length_u != 0) {
    needs_block_extension = true;
    std::get<3>(extends_in_direction) = true;
  }

  extensions = {extension_length_t, extension_length_s, extension_length_v,
      extension_length_u};
}


template<typename PelType>
void JPLM4DTransformModeLightFieldCodec<PelType>::run() {
  const auto& block_coordinates_and_sizes = get_block_coordinates_and_sizes();
  for (auto&& [position, size] : block_coordinates_and_sizes) {
    auto lf_coordinate = LightfieldCoordinate<uint32_t>(position);
    const auto& [t, s, v, u] = lf_coordinate;

    if (transform_mode_configuration.is_verbose()) {
      std::cout << "Transforming 4D at " << t << ", " << s << ", " << v << ", "
                << u << std::endl;
    }

    const auto number_of_colour_channels =
        this->get_number_of_colour_components();
    for (auto color_channel_index = decltype(number_of_colour_channels){0};
         color_channel_index < number_of_colour_channels;
         ++color_channel_index) {
      run_for_block_4d(color_channel_index, lf_coordinate, size);
    }
  }

  finalization();
  if (transform_mode_configuration.is_verbose()) {
    std::cout << "DONE" << std::endl;
  }
}


#endif /* end of include guard: JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__ */