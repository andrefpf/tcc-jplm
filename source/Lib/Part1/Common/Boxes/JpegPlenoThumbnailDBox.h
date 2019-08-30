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

/** \file     JpegPlenoThumbnailDBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOTHUMBNAILDBOX_H__
#define JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOTHUMBNAILDBOX_H__

#include "JpegPlenoThumbnailContents.h"
#include "source/Lib/Common/Boxes/DBox.h"


class JpegPlenoThumbnailDBox : public DBox {
 public:
  JpegPlenoThumbnailDBox(const JpegPlenoThumbnailContents& contents)
      : DBox(std::make_unique<JpegPlenoThumbnailContents>(contents)) {
  }


  JpegPlenoThumbnailDBox(const JpegPlenoThumbnailDBox& other)
      : DBox(std::make_unique<JpegPlenoThumbnailContents>(
            other.get_ref_to_contents())) {
  }


  virtual const JpegPlenoThumbnailContents& get_ref_to_contents()
      const override {
    return static_cast<const JpegPlenoThumbnailContents&>(*contents);
  }


  ~JpegPlenoThumbnailDBox() = default;


  JpegPlenoThumbnailDBox* clone() const override {
    return new JpegPlenoThumbnailDBox(*this);
  }


  friend void swap(JpegPlenoThumbnailDBox& thumbnail_a,
      JpegPlenoThumbnailDBox& thumbnail_b) {
    using std::swap;

    swap(thumbnail_a.contents, thumbnail_b.contents);
  }


  JpegPlenoThumbnailDBox& operator=(const JpegPlenoThumbnailDBox& other) {
    if (&other == this)
      return *this;

    JpegPlenoThumbnailDBox temp{other};
    swap(*this, temp);

    return *this;
  }
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_BOXES_JPEGPLENOTHUMBNAILDBOX_H__ */