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

/** \file     ColourSpecificationBoxParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-28
 */

#include "ColourSpecificationBoxParser.h"

std::unique_ptr<Box> JPLMBoxParser::ColourSpecificationBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto meth = box_parser_helper.get_next<uint8_t>();
  auto prec = box_parser_helper.get_next<int8_t>();
  auto approx = box_parser_helper.get_next<uint8_t>();


  if (meth == 1) {
    auto enum_cs = box_parser_helper.get_next<enum_cs_field_type>();
    auto colour_specification_box =
        std::make_unique<ColourSpecificationBox>(ColourSpecificationContents(meth, prec, approx, EnumCS{enum_cs}));

    return colour_specification_box;
  }

  //! \todo implement ICCProfile parsing (needs a ICC Profile implementation available)
  if (meth == 2) {
    // throw BoxParsersExceptions::ColourSpecificationBoxExceptions::ICCProfileIsNotSupportedYetException();
  // 	auto icc_profile = BoxParserRegistry::get_instance().parse<ICCProfile>(box_parser_helper.get_remaining_stream());
  // 	auto colour_specification_box =
  //       std::make_unique<ColourSpecificationBox>(ColourSpecificationContents(meth, prec, approx, icc_profile));

  //   return colour_specification_box;
  }

  //throw (the only options are 1 and 2 for meth)
  return nullptr;
}