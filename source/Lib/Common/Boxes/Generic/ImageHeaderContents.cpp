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

/** \file     ImageHeaderContents.cpp
 *  \brief    Brief description
 *  \details  Detailed description
 *  \author   Pedro Garcia Freitas <pedro.gf@samsung.com>
 *  \date     2020-02-06
 */

#include "Lib/Common/Boxes/Generic/ImageHeaderContents.h"

ImageHeaderContents::ImageHeaderContents(uint32_t height, uint32_t width,
    uint16_t number_of_channels, uint8_t bits_per_component,
    CompressionTypeImage coder_type, uint8_t UnkC, uint8_t IPR)
    : height(height), width(width), nc(number_of_channels),
      bpc(bits_per_component), c(coder_type), UnkC(UnkC), IPR(IPR) {
  //should width, height and bpc be checked against 0?
  if ((height == 0) || (width == 0) || (nc == 0) || (bpc == 0)) {
    throw ImageHeaderBoxExceptions::InvalidSizeException(
        height, width, nc, bpc);
  }
}

uint32_t ImageHeaderContents::get_height() const noexcept {
  return height;
}

uint32_t ImageHeaderContents::get_width() const noexcept {
  return width;
}

uint16_t ImageHeaderContents::get_nc() const noexcept {
  return nc;
}

uint16_t ImageHeaderContents::get_number_of_channels() const noexcept {
  return get_nc();
}

uint8_t ImageHeaderContents::get_bpc() const noexcept {
  return bpc;
}

uint8_t ImageHeaderContents::get_bits_per_component() const noexcept {
  return get_bpc();
}

CompressionTypeImage ImageHeaderContents::get_c() const noexcept {
  return c;
}

CompressionTypeImage ImageHeaderContents::get_coder_type() const noexcept {
  return get_c();
}

bool ImageHeaderContents::has_known_color_space() const noexcept {
  if (UnkC == 0) {  // 0, if the colourspace of the image is known
    return true;
  }
  //! \todo Check if should throw exception when value is not 0 or 1
  // Values other than 0 and 1 are reserved for ISO use
  return false;
}

bool ImageHeaderContents::has_intellectual_property() const noexcept {
  if (UnkC == 0) {
    return false;
  }  //1
  return true;
  //! \todo Check if should throw exception when value is not 0 or 1
  // Other values are reserved for ISO use
}

ImageHeaderContents *ImageHeaderContents::clone() const {
  return new ImageHeaderContents(*this);
}

uint64_t ImageHeaderContents::size() const noexcept {
  return 2 * sizeof(uint32_t) + sizeof(uint16_t) + 3 * sizeof(uint8_t) +
         sizeof(compression_type_data);
}

bool ImageHeaderContents::is_equal(const DBox &other) const {
  if (typeid(*this) != typeid(other))
    return false;
  const auto &cast_other = dynamic_cast<const ImageHeaderContents &>(other);
  return *this == cast_other;
}

bool ImageHeaderContents::operator==(const ImageHeaderContents &other) const {
  return std::tie(this->height, this->width, this->nc, this->bpc, this->c,
             this->UnkC, this->IPR) == std::tie(other.height, other.width,
                                           other.nc, other.bpc, other.c,
                                           other.UnkC, other.IPR);
}

bool ImageHeaderContents::operator!=(const ImageHeaderContents &other) const {
  return !this->operator==(other);
}
