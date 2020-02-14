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

/** \file     ColourComponentScalingMarkerSegment.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-12
 */
#include "Lib/Part2/Common/TransformMode/ColourComponentScalingMarkerSegment.h"


ColourComponentScalingMarkerSegment::ColourComponentScalingMarkerSegment(
    bool has_more_than_256_colour_components,
    std::size_t colour_component_index, uint16_t Spscc)
    : more_than_256_colour_components(has_more_than_256_colour_components),
      colour_component(
          has_more_than_256_colour_components
          ? std::variant<uint8_t, uint16_t>(
              static_cast<uint16_t>(colour_component_index))
          : std::variant<uint8_t, uint16_t>(
              static_cast<uint8_t>(colour_component_index))),
      exponent(get_exponent_from_Spscc(Spscc)),
      mantissa(get_mantissa_from_Spscc(Spscc)) {
}


ColourComponentScalingMarkerSegment::ColourComponentScalingMarkerSegment(
    std::size_t NC, std::size_t colour_component_index, uint16_t Spscc)
    : ColourComponentScalingMarkerSegment(
    NC > 256 ? true : false, colour_component_index, Spscc) {
}


uint8_t ColourComponentScalingMarkerSegment::get_exponent() {
  return exponent;
}


uint16_t ColourComponentScalingMarkerSegment::get_mantissa() {
  return mantissa;
}


std::variant<uint8_t, uint16_t>
ColourComponentScalingMarkerSegment::get_colour_component_index() {
  return colour_component;
}


bool ColourComponentScalingMarkerSegment::has_more_than_256_colour_components() {
  return more_than_256_colour_components;
}


double ColourComponentScalingMarkerSegment::get_scaling_factor() {
  return std::pow(2.0, exponent - 16.0) * static_cast<double>(mantissa);
}
