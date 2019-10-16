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

/** \file     LightFieldConfigurationMarkerSegment.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-10-15
 */

#ifndef LIGHTFIELDCONFIGURATIONMARKERSEGMENT_H__
#define LIGHTFIELDCONFIGURATIONMARKERSEGMENT_H__

#include <cmath>
#include <cstdint>
#include <vector>
#include "Lib/Part2/Common/TransformMode/Markers.h"
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "Lib/Utils/Stream/BinaryTools.h"
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCode.h"
#include "Lib/Part2/Common/TransformMode/ComponentSsizParameter.h"



class LightFieldConfigurationMarkerSegment {
 protected:
  static constexpr auto marker_code = Marker::LFC;
  static constexpr uint8_t SLlfc = 0;
  // uint16_t Llfc; //42 to 32808
  LightfieldDimension<uint32_t>
      lightfield_dimension;  //ROWS, COLUMNS, HEIGHT, WIDTH
  // uint16_t NC; //!< NC, number of (colour) components
  std::vector<ComponentSsizParameter>
      Ssiz_vector;  //!< Ssiz^i (NC is the size of Ssiz_vector)
  // uint32_t number_of_4d_blocks; //!< N_4D;
  LightfieldDimension<uint32_t> block_dimension;  //!< BLOCK-SIZE_ (t, s, v, u)
  std::vector<uint8_t> max_bitplane;
  bool truncate; //!< TRNC

 public:
  LightFieldConfigurationMarkerSegment(const LightfieldDimension<uint32_t>& lightfield_dimension, 
                const std::vector<ComponentSsizParameter>& Ssiz,
                const LightfieldDimension<uint32_t>& block_dimension, 
                const std::vector<uint8_t>& max_bitplane,
                bool truncate) : lightfield_dimension(lightfield_dimension),
  Ssiz_vector(Ssiz), block_dimension(block_dimension), truncate(truncate) {
  }


  //! \todo Create a move constructor for LightFieldConfigurationMarkerSegment


  virtual ~LightFieldConfigurationMarkerSegment() = default;


  uint16_t get_number_of_colour_components() const noexcept {
    return Ssiz_vector.size();
  }


  uint16_t get_length_of_marker_segment() const noexcept {
    return 40 + 2 * get_number_of_colour_components();
  }


  uint32_t get_number_of_4d_blocks() const noexcept {
    const auto& [T, S, V, U] = lightfield_dimension;
    const auto& [BLOCK_SIZE_t, BLOCK_SIZE_s, BLOCK_SIZE_v, BLOCK_SIZE_u] =
        block_dimension;
    return static_cast<uint32_t>(std::ceil(T / ((double) BLOCK_SIZE_t)) *
                                 std::ceil(S / ((double) BLOCK_SIZE_s)) *
                                 std::ceil(V / ((double) BLOCK_SIZE_v)) *
                                 std::ceil(U / ((double) BLOCK_SIZE_u)));
  }


  std::vector<std::byte> to_bytes() const {
    auto bytes = std::vector<std::byte>();
    bytes.reserve(this->get_length_of_marker_segment()+2); //+2 is because it will include the marker
    BinaryTools::byte_vector_cat(bytes, Markers::get_bytes(LightFieldConfigurationMarkerSegment::marker_code));
    bytes.push_back(std::byte{LightFieldConfigurationMarkerSegment::SLlfc});
    BinaryTools::append_big_endian_bytes(bytes, get_length_of_marker_segment());
    BinaryTools::append_big_endian_bytes(bytes, lightfield_dimension.as_tuple());
    BinaryTools::append_big_endian_bytes(bytes, get_number_of_colour_components());
    for(const auto& Ssiz: Ssiz_vector) {
      BinaryTools::append_big_endian_bytes(bytes, Ssiz());
    }
    BinaryTools::append_big_endian_bytes(bytes, get_number_of_4d_blocks());
    BinaryTools::append_big_endian_bytes(bytes, max_bitplane);
    bytes.push_back(std::byte{truncate});
    return bytes;
  }


};

#endif /* end of include guard: LIGHTFIELDCONFIGURATIONMARKERSEGMENT_H__ */