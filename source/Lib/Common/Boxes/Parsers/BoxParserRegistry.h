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

/** \file     BoxParserRegistry.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSER_H__
#define JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSER_H__

#include <cstdio>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include "Lib/Common/Boxes/Box.h"
#include "Lib/Common/Boxes/Parsers/ColourSpecificationBoxParser.h"
#include "Lib/Common/Boxes/Parsers/ContiguousCodestreamBoxParser.h"
#include "Lib/Part1/Decoder/Boxes/FileTypeBoxParser.h"
#include "Lib/Part1/Decoder/Boxes/JpegPlenoSignatureBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/JpegPlenoLightFieldHeaderBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/LightFieldHeaderBoxParser.h"
#include "Lib/Part2/Decoder/Boxes/ProfileAndLevelBoxParser.h"
#include "Lib/Utils/Stream/ManagedStream.h"

class BoxParserRegistry {
  using ParsedBox = std::unique_ptr<Box>;
  using ParsingFunction = std::function<ParsedBox(BoxParserHelperBase&)>;

 private:
  void register_known_parsers();


  BoxParserRegistry() {
    register_known_parsers();
  }


  ~BoxParserRegistry() = default;

 public:
  static BoxParserRegistry& get_instance();


  static std::map<uint32_t, ParsingFunction>& get_ref_to_parser_map();


  ParsedBox parse(ManagedStream&& managed_stream) const;


  template<class ParsingBox, bool required = true>
  std::unique_ptr<ParsingBox> parse(ManagedStream&& managed_stream) const {
    // std::cout << "ManagedStream is at: " << managed_stream.tell() << std::endl;
    // std::cout << "ManagedStream is limited to: " << managed_stream.get_length() << std::endl;
    // std::cout << typeid(ParsingBox).name() << std::endl;
    auto box_parser_helper =
        BoxParserHelper<ParsingBox, required>(managed_stream);
    // std::cout << "created helper here <>" << std::endl;
    if constexpr (!required) {
      if (!box_parser_helper.is_a_box_with_id(ParsingBox::id)) {
        //this means that the current box being read is not of the type ParsingBox
        //therefore, it can rewind the stream to allow for a next box to read again the l and t boxes
        managed_stream.rewind();
        return nullptr;
      }
    }
    auto parsed_box = parse(box_parser_helper);
    if (parsed_box) {  //i.e., the unique_ptr is not null
      return std::unique_ptr<ParsingBox>(
          static_cast<ParsingBox*>(parsed_box.release()));
    }
    return nullptr;
  }


  ParsedBox parse(BoxParserHelperBase& box_parser_helper) const;


  template<class ParserClass>
  void register_parser() {
    auto& map = BoxParserRegistry::get_ref_to_parser_map();
    const auto id = ParserClass::ParsingBox::id;
    auto parsing_function = ParserClass::parse;
    // std::cout << "Registered 0x" << std::hex << std::setfill('0')
    //           << std::setw(8) << id << std::endl;
    map[id] = parsing_function;
  }
};

#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_PARSERS_BOXPARSER_H__ */