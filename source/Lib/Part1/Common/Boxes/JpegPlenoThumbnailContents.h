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

/** \file     JpegPlenoThumbnailContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOTHUMBNAILCONTENTS_H__
#define JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOTHUMBNAILCONTENTS_H__

#include <algorithm>
#include <numeric>
#include "Lib/Common/Boxes/Generic/BitsPerComponentBox.h"
#include "Lib/Common/Boxes/Generic/ImageHeaderContents.h"
#include "Lib/Common/Boxes/Generic/ContiguousCodestreamBox.h"
#include "Lib/Common/Boxes/Generic/ColourSpecificationBox.h"
#include "Lib/Common/Boxes/Generic/ChannelDefinitionBox.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"

class JpegPlenoThumbnailContents : public InMemoryDBox {
 protected:
  ImageHeaderBox ihdr;  //image header box
  BitsPerComponentBox bpcc;
  std::vector<ColourSpecificationBox> colr;
  ChannelDefinitionBox cdef;
  ContiguousCodestreamBox jpc2;

 public:
  JpegPlenoThumbnailContents() = default;

  
  JpegPlenoThumbnailContents(const JpegPlenoThumbnailContents& other)
      : ihdr(other.ihdr), bpcc(other.bpcc), colr(other.colr), cdef(other.cdef),
        jpc2(other.jpc2) {
  }


  JpegPlenoThumbnailContents(JpegPlenoThumbnailContents&& other)
      : ihdr(std::move(other.ihdr)), bpcc(std::move(other.bpcc)),
        colr(std::move(other.colr)), cdef(std::move(other.cdef)),
        jpc2(std::move(other.jpc2)) {
  }


  virtual JpegPlenoThumbnailContents* clone() const override {
    return new JpegPlenoThumbnailContents(*this);
  }


  virtual uint64_t size() const noexcept override {
    auto sum_colr_sizes = [](uint64_t sum, const ColourSpecificationBox& val) {
      return sum + val.size();
    };

    return ihdr.size() + bpcc.size() +
           std::accumulate(colr.begin(), colr.end(), 0, sum_colr_sizes) +
           cdef.size() + jpc2.size();
  }


  virtual bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    const auto& cast_other =
        dynamic_cast<const JpegPlenoThumbnailContents&>(other);
    return *this == cast_other;
  }


  bool operator==(const JpegPlenoThumbnailContents& other) const {
    return std::tie(
               this->ihdr, this->bpcc, this->colr, this->cdef, this->jpc2) ==
           std::tie(other.ihdr, other.bpcc, other.colr, other.cdef, other.jpc2);
  }


  bool operator!=(const JpegPlenoThumbnailContents& other) const {
    return !this->operator==(other);
  }
};

using JpegPlenoThumbnailBox = GenericBox<0x6a707468, JpegPlenoThumbnailContents>;

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOTHUMBNAILCONTENTS_H__ */