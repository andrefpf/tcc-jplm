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

/** \file     CodestreamPointerSetMarkerSegment.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-11
 */
#include "Lib/Part2/Common/TransformMode/CodestreamPointerSetMarkerSegment.h"


std::size_t CodestreamPointerSetMarkerSegment::get_number_of_pointers() const {
  return PPnt.size();
}


std::variant<uint32_t, uint64_t>
CodestreamPointerSetMarkerSegment::get_pointer_at(std::size_t i) const {
  return PPnt.at(i);
}


uint64_t CodestreamPointerSetMarkerSegment::get_length_of_marker_segment()
    const {
  if (std::holds_alternative<uint32_t>(PPnt[0])) {
    return 9 + PPnt.size() * sizeof(uint32_t);
  }
  return 9 + PPnt.size() * sizeof(uint64_t);
}


std::vector<std::byte> CodestreamPointerSetMarkerSegment::get_bytes() const {
  auto marker_bytes =
      Markers::get_bytes(CodestreamPointerSetMarkerSegment::marker_code);
  auto bytes = std::vector<std::byte>(marker_bytes.begin(), marker_bytes.end());
  //2
  BinaryTools::append_big_endian_bytes(
      bytes, CodestreamPointerSetMarkerSegment::SLpnt);
  //3
  BinaryTools::append_big_endian_bytes(bytes, get_length_of_marker_segment());
  //11
  if (std::holds_alternative<uint32_t>(PPnt[0])) {
    BinaryTools::append_big_endian_bytes(bytes, static_cast<uint8_t>(1));
  } else {
    BinaryTools::append_big_endian_bytes(bytes, static_cast<uint8_t>(2));
  }
  //12
  for (const auto& Ppnt_element : PPnt) {
    std::visit(
        [&bytes](
            const auto& v) { BinaryTools::append_big_endian_bytes(bytes, v); },
        Ppnt_element);
  }
  assert((bytes.size() - 3) == get_length_of_marker_segment());
  std::cout << bytes.size() << std::endl;
  return bytes;
}