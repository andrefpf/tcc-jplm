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

/** \file     BoxParserRegistry.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-14
 */

#include "BoxParserRegistry.h"


BoxParserRegistry& BoxParserRegistry::get_instance() {
  static BoxParserRegistry instance;
  return instance;
}


std::map<uint32_t, BoxParserRegistry::ParsingFunction>&
BoxParserRegistry::get_ref_to_parser_map() {
  static std::map<uint32_t, BoxParserRegistry::ParsingFunction> parser_map;
  return parser_map;
}


// auto box_parser_helper = BoxParserHelper<ParsingBox>(managed_stream);
BoxParserRegistry::ParsedBox BoxParserRegistry::parse(
    ManagedStream&& managed_stream) const {
  auto box_parser_helper = BoxParserHelperBase(managed_stream);
  // std::cout << "called parser " << std::endl;
  return parse(box_parser_helper);
}


BoxParserRegistry::ParsedBox BoxParserRegistry::parse(
    BoxParserHelperBase& box_parser_helper) const {
  auto& map = BoxParserRegistry::get_ref_to_parser_map();
  if (auto it = map.find(box_parser_helper.get_t_box_value());
      it != map.end()) {
    // std::cout << "found parsing method for id 0x" << std::hex
    //           << std::setfill('0') << std::setw(8)
    //           << box_parser_helper.get_t_box_value() << std::dec << std::endl;
    // std::cout << "Before parsing: " << box_parser_helper.
    // here i need to limit the access
    auto box_parser_helper_with_protected_range = box_parser_helper.get_helper_with_protected_range();
    return it->second(box_parser_helper_with_protected_range);
  }
  //not found a parser... should go to the end of the managed stream
  box_parser_helper.get_data_stream()
      .forward();  //a compliant decoder should ignore unknown boxes
  return nullptr;
}


void BoxParserRegistry::register_known_parsers() {
  //add here known parsers
  using namespace JPLMBoxParser;
  // Common:
  register_parser<ColourSpecificationBoxParser>();
  // Part 1:
  //! [Registering the parser]
  register_parser<JpegPlenoSignatureBoxParser>();
  //! [Registering the parser]
  register_parser<FileTypeBoxParser>();
  // Part 2:
  register_parser<ProfileAndLevelBoxParser>();
  register_parser<LightFieldHeaderBoxParser>();
  register_parser<JpegPlenoLightFieldHeaderBoxParser>();
  register_parser<JpegPlenoLightFieldBoxParser>();
  register_parser<ContiguousCodestreamBoxParser>();
  // register_parser<...>();  
}
