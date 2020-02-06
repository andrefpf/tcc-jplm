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

/** \file     ImageHeaderContents.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-08-21
 */

#ifndef JPLM_LIB_COMMON_BOXES_GENERIC_IMAGEHEADERCONTENTS_H__
#define JPLM_LIB_COMMON_BOXES_GENERIC_IMAGEHEADERCONTENTS_H__

#include <tuple>  //std::tie
#include "CompressionTypeImage.h"
#include "Lib/Common/Boxes/InMemoryDBox.h"
#include "Lib/Part1/Common/CommonExceptions.h"


class ImageHeaderContents : public InMemoryDBox {
 protected:
  uint32_t height;  //!< Image height
  uint32_t width;  //!< Image width
  uint16_t nc;  //!< Number of components
  uint8_t bpc;  //!< Bits per component
  CompressionTypeImage c;  //!< Compression type
  uint8_t UnkC;  //!< Colourspace Unknown
  uint8_t IPR;  //!< Intellectual Property

 public:
  ImageHeaderContents(uint32_t height, uint32_t width,
      uint16_t number_of_channels, uint8_t bits_per_component,
      CompressionTypeImage coder_type, uint8_t UnkC, uint8_t IPR);


  uint32_t get_height() const noexcept;


  uint32_t get_width() const noexcept;


  uint16_t get_nc() const noexcept;


  uint16_t get_number_of_channels() const noexcept;


  uint8_t get_bpc() const noexcept;


  uint8_t get_bits_per_component() const noexcept;


  CompressionTypeImage get_c() const noexcept;


  CompressionTypeImage get_coder_type() const noexcept;


  bool has_known_color_space() const noexcept;


  bool has_intellectual_property() const noexcept;


  virtual ImageHeaderContents* clone() const override;


  virtual ~ImageHeaderContents() = default;


  virtual uint64_t size() const noexcept override;


  virtual bool is_equal(const DBox& other) const override;


  bool operator==(const ImageHeaderContents& other) const;


  bool operator!=(const ImageHeaderContents& other) const;
};


#endif /* end of include guard: JPLM_LIB_COMMON_BOXES_GENERIC_IMAGEHEADERCONTENTS_H__ */