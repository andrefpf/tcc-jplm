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

/** \file     CodestreamPointerSetMarkerSegment.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2020-01-03
 */

#ifndef CODESTREAMPOINTERSETMARKERSEGMENT_H__
#define CODESTREAMPOINTERSETMARKERSEGMENT_H__


#include <cmath>
#include <cstdint>
#include <variant>
#include <vector>
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamCode.h"
#include "Lib/Part2/Common/TransformMode/Markers.h"
#include "Lib/Utils/Stream/BinaryTools.h"


class CodestreamPointerSetMarkerSegment {
 protected:
  static constexpr auto marker_code = Marker::PNT;

  std::vector<std::variant<uint32_t, uint64_t>> PPnt;

 public:
  static constexpr uint8_t SLpnt = 2;
  CodestreamPointerSetMarkerSegment(
      const std::vector<std::variant<uint32_t, uint64_t>>& PPnt)
      : PPnt(PPnt) {
  }


  CodestreamPointerSetMarkerSegment(
      std::vector<std::variant<uint32_t, uint64_t>>&& PPnt)
      : PPnt(std::move(PPnt)) {
  }


  ~CodestreamPointerSetMarkerSegment() = default;


  std::size_t get_number_of_pointers();


  std::variant<uint32_t, uint64_t> get_pointer_at(std::size_t i);
};


#endif /* end of include guard: CODESTREAMPOINTERSETMARKERSEGMENT_H__ */