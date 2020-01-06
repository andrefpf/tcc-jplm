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

/** \file     ColourComponentScalingMarkerSegment.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-01-06
 */

#ifndef COLOURCOMPONENTSCALINGMARKERSEGMENT_H__
#define COLOURCOMPONENTSCALINGMARKERSEGMENT_H__

#include <cmath>
#include <cstdint>
#include <variant>
#include <vector>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCode.h"
#include "Lib/Part2/Common/TransformMode/Markers.h"
#include "Lib/Utils/Stream/BinaryTools.h"

class ColourComponentScalingMarkerSegment {
 protected:
  static constexpr auto marker_code = Marker::SCC;
  bool more_than_256_colour_components;
  std::variant<uint8_t, uint16_t> colour_component;
  uint8_t exponent;  // 0...   31
  uint16_t mantissa;  // 0... 2047

  uint8_t get_exponent_from_Spscc(uint16_t Spscc) {
    uint8_t exp = (Spscc >> 11) & 0x1F;  // 0x1F == 0001 1111
    return exp;
  }


  uint16_t get_mantissa_from_Spscc(uint16_t Spscc) {
    return (Spscc & 0x7FF);
  }

 public:
  static constexpr uint8_t SLscc = 0;

  ColourComponentScalingMarkerSegment(bool has_more_than_256_colour_components,
      std::size_t colour_component_index, uint16_t Spscc)
      : more_than_256_colour_components(
            has_more_than_256_colour_components),
        colour_component(
            has_more_than_256_colour_components
                ? std::variant<uint8_t, uint16_t>(
                      static_cast<uint16_t>(colour_component_index))
                : std::variant<uint8_t, uint16_t>(
                      static_cast<uint8_t>(colour_component_index))),
        exponent(get_exponent_from_Spscc(Spscc)),
        mantissa(get_mantissa_from_Spscc(Spscc)) {
  }


  ColourComponentScalingMarkerSegment(
      std::size_t NC, std::size_t colour_component_index, uint16_t Spscc)
      : ColourComponentScalingMarkerSegment(
            NC > 256 ? true : false, colour_component_index, Spscc) {
  }

  ~ColourComponentScalingMarkerSegment() = default;


  auto get_exponent() {
    return exponent;
  }


  auto get_mantissa() {
    return mantissa;
  }


  auto get_colour_component_index() {
    return colour_component;
  }


  bool has_more_than_256_colour_components() {
  	return more_than_256_colour_components;
  }


  double get_scaling_factor() {
    return std::pow(2.0, exponent - 16.0) * static_cast<double>(mantissa);
  }
};

#endif /* end of include guard: COLOURCOMPONENTSCALINGMARKERSEGMENT_H__ */