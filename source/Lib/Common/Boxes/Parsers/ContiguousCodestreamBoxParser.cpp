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

/** \file     ContiguousCodestreamBoxParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-29
 */

#include "ContiguousCodestreamBoxParser.h"


uint64_t JPLMBoxParser::ContiguousCodestreamBoxParser::memory_limit = std::numeric_limits<uint64_t>::max();


std::unique_ptr<Box> JPLMBoxParser::ContiguousCodestreamBoxParser::parse(
    BoxParserHelperBase& box_parser_helper) {
  auto data_lenght = box_parser_helper.get_data_lenght();

  if (data_lenght <= memory_limit) {
    auto remaining_stream = box_parser_helper.get_remaining_stream();
    assert(remaining_stream.get_length() == data_lenght);
    auto contiguous_codestream_code =
        std::make_unique<ContiguousCodestreamCodeInMemory>(
            remaining_stream.get_n_bytes(data_lenght));
    auto contiguous_codestream_contents = std::make_unique<ContiguousCodestreamContents>(
        std::move(contiguous_codestream_code));

    return std::make_unique<ContiguousCodestreamBox>(std::move(contiguous_codestream_contents));
  }  //else

  // \todo read in a code as pointer to file, or copy to a temporary file...
  //return ;

  return nullptr;
}