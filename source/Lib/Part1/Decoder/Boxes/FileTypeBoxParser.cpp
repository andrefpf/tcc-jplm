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

/** \file     FileTypeBoxParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-19
 */


#include "FileTypeBoxParser.h"

//! \todo how to adere to the following restriction? All files shall contain one and only one FileTypeBox

std::unique_ptr<Box> JPLMBoxParser::FileTypeBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  //! [Parsing a file type box]
  auto brand = box_parser_helper.get_next<uint32_t>();
  auto minor_version = box_parser_helper.get_next<uint32_t>();

  auto compatibility_list = std::vector<uint32_t>();
  auto length = (box_parser_helper.get_data_length() - sizeof(brand) -
                  sizeof(minor_version))/sizeof(uint32_t);
  compatibility_list.reserve(length);

  for(auto i=decltype(length){0}; i < length; ++i) {
    compatibility_list.emplace_back(box_parser_helper.get_next<uint32_t>());  	
  }

  auto file_type_box = std::make_unique<FileTypeBox>(
      FileTypeContents(brand, minor_version, std::move(compatibility_list)));

  return file_type_box;
  //! [Parsing a file type box]
}