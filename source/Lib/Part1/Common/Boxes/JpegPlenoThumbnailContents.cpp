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

/** \file     JpegPlenoThumbnailContents.cpp
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-07
 */

#include "Lib/Part1/Common/Boxes/JpegPlenoThumbnailContents.h"


JpegPlenoThumbnailContents *JpegPlenoThumbnailContents::clone() const {
  return new JpegPlenoThumbnailContents(*this);
}


uint64_t JpegPlenoThumbnailContents::size() const noexcept {
  auto sum_colr_sizes = [](uint64_t sum, const ColourSpecificationBox &val) {
    return sum + val.size();
  };

  return ihdr.size() + bpcc->size() +
         std::accumulate(colr.begin(), colr.end(), 0, sum_colr_sizes) +
         cdef->size() + jpc2->size();
}


bool JpegPlenoThumbnailContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other =
      dynamic_cast<const JpegPlenoThumbnailContents &>(other);
  return *this == cast_other;
}


bool JpegPlenoThumbnailContents::operator==(
    const JpegPlenoThumbnailContents &other) const {
  return std::tie(this->ihdr, this->bpcc, this->colr, this->cdef, this->jpc2) ==
         std::tie(other.ihdr, other.bpcc, other.colr, other.cdef, other.jpc2);
}


bool JpegPlenoThumbnailContents::operator!=(
    const JpegPlenoThumbnailContents &other) const {
  return !this->operator==(other);
}
