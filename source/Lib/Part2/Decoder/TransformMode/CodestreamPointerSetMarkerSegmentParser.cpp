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

/** \file     CodestreamPointerSetMarkerSegmentParser.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-12
 */
#include "Lib/Part2/Decoder/TransformMode/CodestreamPointerSetMarkerSegmentParser.h"

CodestreamPointerSetMarkerSegment CodestreamPointerSetMarkerSegmentParser::
    get_codestream_pointer_set_marker_segment(
        const ContiguousCodestreamCode &codestream_code) {
  auto SLpnt_from_codestream_code =
      MarkerSegmentHelper::get_next<uint8_t>(codestream_code);
  if (SLpnt_from_codestream_code != CodestreamPointerSetMarkerSegment::SLpnt) {
    //throw error
  }

  auto Lpnt_from_codestream_code =
      MarkerSegmentHelper::get_next<uint64_t>(codestream_code);
  //limits 9 to 8x(2^32-1);

  auto Spnt = MarkerSegmentHelper::get_next<uint8_t>(codestream_code);

  auto bytes_per_pointer = (Spnt == 0) ? 4 : 8;

  auto number_of_pointers_in_pnt_segment =
      (Lpnt_from_codestream_code - sizeof(Lpnt_from_codestream_code)) /
      bytes_per_pointer;

  auto ppnts = std::vector<std::variant<uint32_t, uint64_t>>();

  if (Spnt == 0) {
    for (auto i = decltype(number_of_pointers_in_pnt_segment){0};
         i < number_of_pointers_in_pnt_segment; ++i) {
      ppnts.push_back(MarkerSegmentHelper::get_next<uint32_t>(codestream_code));
    }
  } else if (Spnt == 1) {
    for (auto i = decltype(number_of_pointers_in_pnt_segment){0};
         i < number_of_pointers_in_pnt_segment; ++i) {
      ppnts.push_back(MarkerSegmentHelper::get_next<uint64_t>(codestream_code));
    }
  } else {
    //throw
  }


  return CodestreamPointerSetMarkerSegment(std::move(ppnts));
}
