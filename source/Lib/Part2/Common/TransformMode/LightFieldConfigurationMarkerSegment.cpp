/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2020, ITU/ISO/IEC
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

/** \file     LightFieldConfigurationMarkerSegment.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-12
 */
#include "Lib/Part2/Common/TransformMode/LightFieldConfigurationMarkerSegment.h"


uint16_t LightFieldConfigurationMarkerSegment::get_number_of_colour_components()
    const noexcept {
  return Ssiz_vector.size();
}


uint16_t LightFieldConfigurationMarkerSegment::get_length_of_marker_segment()
    const noexcept {
  // in the standard it says 40+2*colour components... But it is not accounting for TRNC
  return 41 + 2 * get_number_of_colour_components();
}


uint32_t LightFieldConfigurationMarkerSegment::get_number_of_4d_blocks() const {
  const auto& [T, S, V, U] = lightfield_dimension;
  const auto& [BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u] =
      block_dimension;
  const auto n_4d =
      static_cast<uint32_t>(std::ceil(T / ((double) BLOCK_SIZE_t)) *
                            std::ceil(S / ((double) BLOCK_SIZE_s)) *
                            std::ceil(V / ((double) BLOCK_SIZE_v)) *
                            std::ceil(U / ((double) BLOCK_SIZE_u)) *
                            get_number_of_colour_components());

  if (n_4d > std::numeric_limits<uint32_t>::max()) {
    throw LightFieldConfigurationMarker::OverflowInTheNumberOf4DBlocks(n_4d);
  }

  return static_cast<uint32_t>(n_4d);
}


std::vector<std::byte> LightFieldConfigurationMarkerSegment::get_bytes() const {
  auto bytes = std::vector<std::byte>();
  bytes.reserve(this->get_length_of_marker_segment() +
                2);  //+2 is because it will include the marker
  BinaryTools::byte_vector_cat(bytes,
      Markers::get_bytes(LightFieldConfigurationMarkerSegment::marker_code));
  bytes.push_back(std::byte{LightFieldConfigurationMarkerSegment::SLlfc});
  BinaryTools::append_big_endian_bytes(bytes, get_length_of_marker_segment());
  BinaryTools::append_big_endian_bytes(bytes, lightfield_dimension.as_tuple());
  BinaryTools::append_big_endian_bytes(
      bytes, get_number_of_colour_components());
  for (const auto& Ssiz : Ssiz_vector) {
    BinaryTools::append_big_endian_bytes(bytes, Ssiz());
  }
  BinaryTools::append_big_endian_bytes(bytes, get_number_of_4d_blocks());
  BinaryTools::append_big_endian_bytes(bytes, block_dimension.as_tuple());
  BinaryTools::append_big_endian_bytes(bytes, max_bitplane);
  bytes.push_back(std::byte{truncate});
  return bytes;
}


uint8_t LightFieldConfigurationMarkerSegment::get_max_bitdepth_at_channel(
    const std::size_t channel) {
  return Ssiz_vector.at(channel)();
}


bool LightFieldConfigurationMarkerSegment::get_truncate_flag() const {
  return truncate;
}


const std::vector<ComponentSsizParameter>&
LightFieldConfigurationMarkerSegment::get_ref_to_bitdepths() {
  return Ssiz_vector;
}


uint8_t LightFieldConfigurationMarkerSegment::get_max_bitplane_at_channel(
    const std::size_t channel) {
  return max_bitplane.at(channel);
}


const std::vector<uint8_t>&
LightFieldConfigurationMarkerSegment::get_ref_to_max_bitplanes() {
  return max_bitplane;
}


const LightfieldDimension<uint32_t>&
LightFieldConfigurationMarkerSegment::get_ref_to_lightfield_dimension() const {
  return lightfield_dimension;
}


const LightfieldDimension<uint32_t>&
LightFieldConfigurationMarkerSegment::get_ref_to_block_dimension() const {
  return block_dimension;
}
