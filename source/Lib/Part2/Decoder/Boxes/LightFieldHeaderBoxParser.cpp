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

/** \file     LightFieldHeaderBoxParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-27
 */


#include "LightFieldHeaderBoxParser.h"

std::unique_ptr<Box> JPLMBoxParser::LightFieldHeaderBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  const auto t = box_parser_helper.get_next<uint32_t>();
  const auto s = box_parser_helper.get_next<uint32_t>();
  const auto v = box_parser_helper.get_next<uint32_t>();
  const auto u = box_parser_helper.get_next<uint32_t>();

  const auto nc = box_parser_helper.get_next<uint16_t>();
  const auto bpc = box_parser_helper.get_next<uint8_t>();
  const auto c = box_parser_helper.get_next<compression_type_data>();
  const auto unkc =
      box_parser_helper.get_next<colour_space_unknown_flag_type>();
  const auto ipr = box_parser_helper.get_next<intelectual_property_flag_type>();

  auto light_field_header_contents = LightFieldHeaderContents({t, s, v, u}, nc,
      bpc, CompressionTypeLightField{c}, ColourSpaceUnknownFlag{unkc},
      IntelectualPropertyFlag{ipr});

  auto light_field_header_box =
      std::make_unique<LightFieldHeaderBox>(std::move(light_field_header_contents));
  return light_field_header_box;
}