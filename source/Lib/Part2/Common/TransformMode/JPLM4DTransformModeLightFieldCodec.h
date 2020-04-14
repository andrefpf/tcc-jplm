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

#ifndef JPLM_LIB_PART2_COMMON_TRANSFORMMODE_JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__
#define JPLM_LIB_PART2_COMMON_TRANSFORMMODE_JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__

#include <cstdint>
#include <iostream>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamBox.h"
#include "Lib/Part2/Common/JPLMLightFieldCodec.h"
#include "Lib/Part2/Common/TransformMode/BorderBlocksPolicy.h"
#include "Lib/Part2/Common/TransformMode/CodestreamPointerSetMarkerSegment.h"
#include "Lib/Part2/Common/TransformMode/CommonExceptions.h"
#include "Lib/Part2/Common/TransformMode/DCT4DCoefficientsManager.h"
#include "Lib/Part2/Common/TransformMode/LightFieldTransformMode.h"
#include "Lib/Utils/Image/ColorSpaces.h"
#include "cppitertools/product.hpp"
#include "cppitertools/zip.hpp"
#include "tqdm-cpp/tqdm.hpp"


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


  /**
   * \brief      Checks the consistency of pnt
   *
   * \param[in]  code  The contigous codestream code
   * \param[in]  pnt   The codestream pointer set marker segment (pnt)
   * 
   * Throws if each pointer is not pointing to a SOB marker.
   * \todo throw if the number of entries is different than the number of 4d blocks
   * 
   */
  void check_consistency_of_pnt(const ContiguousCodestreamCode& code,
      const CodestreamPointerSetMarkerSegment& pnt) const {
    auto n_pointers = pnt.get_number_of_pointers();

    for (auto i = decltype(n_pointers){0}; i < n_pointers; ++i) {
      auto index_var = pnt.get_pointer_at(i);
      auto previous_index = uint64_t(0);
      std::visit(
          [&code, &previous_index](auto index) {
            if (index < previous_index) {
              //advancing in the bitstream cannot have a smaller value
              //if this happened it was most probably caused by an overflow
              throw CodestreamPointerSetMarker::OverflowInTheEntriesOfPNT();
            }
            previous_index = index;
            constexpr uint64_t box_lenght_and_type_size = 8;

            if (!((code.get_byte_at(index - box_lenght_and_type_size) ==
                      std::byte{0xFF}) &&
                    (code.get_byte_at(index - box_lenght_and_type_size + 1) ==
                        std::byte{0xA4}))) {
              throw CodestreamPointerSetMarker::
                  CodestreamPointerSetEntryIsNotPointingToSOB(index);
            }
          },
          index_var);
    }
  }


  /**
   * @brief      Gets the number of colour components.
   *
   * @return     The number of colour components.
   */
  virtual uint16_t get_number_of_colour_components() const = 0;


  /**
   * @brief      Gets the vector of positions in dimension.
   *
   * @param[in]  total_size  The total size of the lightfield in the dimension.
   * @param[in]  block_size  The 4D block size in the dimension.
   *
   * @return     The vector of positions in dimension.
   */
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


  /**
   * @brief      Gets the block coordinates and sizes used to iterate over.
   *
   * @return     The block coordinates and sizes.
   */
  auto get_block_coordinates_and_sizes() const {
    const auto& [T, S, V, U] = lightfield_dimension;
    // std::cout << "Lf dimension: " << lightfield_dimension << std::endl;
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


    auto channels_vector =
        std::vector<uint16_t>(this->get_number_of_colour_components());
    std::iota(channels_vector.begin(), channels_vector.end(), 0);


    const auto& boder_blocks_policy = this->get_border_blocks_policy();
    if (boder_blocks_policy == BorderBlocksPolicy::truncate) {
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
      // std::cout << "truncate v: " << v_sizes.back() << std::endl;
      // std::cout << "truncate u: " << u_sizes.back() << std::endl;
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


    auto&& with_channel = iter::product(zip_iter, channels_vector);


    std::vector<std::tuple<LightfieldCoordinate<uint32_t>,
        LightfieldDimension<uint32_t>, uint16_t>>
        ret_vect;

    std::transform(with_channel.begin(), with_channel.end(),
        std::back_inserter(ret_vect), [](const auto&& in) {
          const auto& coordinate_and_size = std::get<0>(in);
          const auto& coordinate =
              LightfieldCoordinate<uint32_t>(std::get<0>(coordinate_and_size));
          const auto& size =
              LightfieldDimension<uint32_t>(std::get<1>(coordinate_and_size));
          auto channel = std::get<1>(in);
          return std::make_tuple(coordinate, size, channel);
        });


    return ret_vect;
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

  if (transform_mode_configuration.show_progress_bar()) {
    for (auto&& [position, size, channel] :
        tq::tqdm(block_coordinates_and_sizes)) {
      run_for_block_4d(channel, position, size);
    }
  } else {
    for (auto&& [position, size, channel] : block_coordinates_and_sizes) {
      if (transform_mode_configuration.is_verbose()) {
        std::cout << "Transforming 4D at " << position << std::endl;
      }
      run_for_block_4d(channel, position, size);
    }
  }


  finalization();
  if (transform_mode_configuration.is_verbose()) {
    std::cout << "DONE" << std::endl;
  }
}


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_TRANSFORMMODE_JPLM4DTRANSFORMMODELIGHTFIELDCODEC_H__ */