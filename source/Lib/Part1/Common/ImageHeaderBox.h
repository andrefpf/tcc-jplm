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

/** \file     ImageHeaderBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-07-24
 */

#ifndef JPLM_LIB_PART1_COMMON_IMAGEHEADERBOX_H__
#define JPLM_LIB_PART1_COMMON_IMAGEHEADERBOX_H__

#include "Box.h"
#include "DefinedBoxes.h"


enum class CoderTypeC : uint8_t {
  JPEG_2000 = 0,
  JPEG = 1,
  JPEG_LS = 2,
  JPEG_XT = 3,
  JPEG_XR = 4,
  JPEG_XS = 5
};


typedef std::underlying_type<CoderTypeC>::type CoderTypeCUnderlyingType;


class ImageHeaderContents : public DBoxContents {
 protected:
  uint32_t height;
  uint32_t width;
  uint16_t nc;
  uint8_t bpc;
  CoderTypeC c;
  uint8_t UnkC;
  uint8_t IPR;

 public:
  ImageHeaderContents() = default;
  ~ImageHeaderContents() = default;

  uint64_t size() const noexcept {
    return 2 * sizeof(uint32_t) + sizeof(uint16_t) + 3 * sizeof(uint8_t) +
           sizeof(CoderTypeCUnderlyingType);
  }


  bool operator==(const ImageHeaderContents& other) const {
    return std::tie(this->height, this->width, this->nc, this->bpc, this->c,
               this->UnkC, this->IPR) == std::tie(other.height, other.width,
                                             other.nc, other.bpc, other.c,
                                             other.UnkC, other.IPR);
  }

  bool operator!=(const ImageHeaderContents& other) const {
    return !this->operator==(other);
  }
};


class ImageHeaderDBox : public DBox {
 public:
  ImageHeaderDBox(const ImageHeaderContents& contents)
      : DBox(std::make_any<ImageHeaderContents>(contents)) {
  }


  ImageHeaderDBox(const ImageHeaderDBox& other)
      : DBox(std::make_any<ImageHeaderContents>(
            std::any_cast<ImageHeaderContents>(other.contents))) {
  }


  ~ImageHeaderDBox() = default;


  uint64_t size() const noexcept override {
    return std::any_cast<ImageHeaderContents>(this->contents).size();
  }


  ImageHeaderDBox* clone() const override {
    return new ImageHeaderDBox(*this);
  }

  
  bool is_equal(const DBox& other) const override {
    if (typeid(*this) != typeid(other))
      return false;
    return (std::any_cast<ImageHeaderContents>(this->get_ref_to_contents()) ==
            std::any_cast<ImageHeaderContents>(other.get_ref_to_contents()));
  }


  virtual std::vector<uint8_t> get_bytes() const noexcept override {
    return std::any_cast<ImageHeaderContents>(this->get_ref_to_contents()).get_bytes();
  }
};


class ImageHeaderBox : public Box {
 public:
  ImageHeaderBox(const ImageHeaderDBox& contents)
      : Box(TBox(static_cast<DefinedBoxesTypesUnderlyingType>(
                DefinedBoxesTypes::ImageHeaderBoxType)),
            contents){};


  ~ImageHeaderBox() = default;
};

#endif /* end of include guard: JPLM_LIB_PART1_COMMON_IMAGEHEADERBOX_H__ */