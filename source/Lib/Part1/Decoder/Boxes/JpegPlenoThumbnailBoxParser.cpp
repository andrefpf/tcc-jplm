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

/** \file     JpegPlenoThumbnailBoxParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-03-24
 */

#include "Lib/Part1/Decoder/Boxes/JpegPlenoThumbnailBoxParser.h"


std::unique_ptr<Box> JPLMBoxParser::JpegPlenoThumbnailBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto& box_parser = BoxParserRegistry::get_instance();

  auto bits_per_component_box =
      box_parser_helper.has_data_available()
          ? box_parser.parse<BitsPerComponentBox, false>(
                box_parser_helper.get_remaining_stream())
          : nullptr;  //optional, may be nullptr


  std::vector<std::unique_ptr<ColourSpecificationBox>> colr;

  while (box_parser_helper.has_data_available()) {
    auto colour_specification_box =
        box_parser.parse<ColourSpecificationBox, false>(
            box_parser_helper.get_remaining_stream());
    if (colour_specification_box) {
      colr.emplace_back(std::move(colour_specification_box));
    } else {
      break;
    }
  }


  if (colr.size() == 0) {
    throw JpegPlenoThumbnailBoxParserExceptions::
        AtLeastOneColorSpecificationBoxShallExistException();
  }

  auto channel_definition_box =
      box_parser_helper.has_data_available()
          ? box_parser.parse<ChannelDefinitionBox, false>(
                box_parser_helper.get_remaining_stream())
          : nullptr;


  auto contigous_codestream_box =
      box_parser_helper.has_data_available()
          ? box_parser.parse<ContiguousCodestreamBox, false>(
                box_parser_helper.get_remaining_stream())
          : nullptr;


  return nullptr;
}