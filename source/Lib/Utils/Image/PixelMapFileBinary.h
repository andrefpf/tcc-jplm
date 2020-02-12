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

/** \file     PixelMapFileBinary.h
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-28
 */

#ifndef JPLM_LIB_UTILS_IMAGE_PIXELMAPFILEBINARY_H__
#define JPLM_LIB_UTILS_IMAGE_PIXELMAPFILEBINARY_H__

#include "Lib/Utils/Image/PixelMapFile.h"

//TODO
// \todo: It is the same function defined at BinaryTools.h
constexpr bool using_little_endian() {
#ifdef _WIN32
  return true;
#else
  if (__BYTE_ORDER__ != __ORDER_BIG_ENDIAN__)
    return true;
  return false;
#endif
}

//P4 to P6 stores images in binary
class PixelMapFileBinary : public PixelMapFile {
 public:
  PixelMapFileBinary(const std::string& file_name, PixelMapType type)
      : PixelMapFile(file_name, type){};


  PixelMapFileBinary(const PixelMapFileBinary& other) = default;


  PixelMapFileBinary(const std::string& file_name, PixelMapType type,
      std::size_t width, std::size_t height, std::size_t max_value)
      : PixelMapFile(file_name, type, width, height, max_value){};


  PixelMapFileBinary(const std::string& file_name,
      const std::streampos raster_begin, PixelMapType type, std::size_t width,
      std::size_t height, std::size_t max_value)
      : PixelMapFile(file_name, raster_begin, type, width, height, max_value){};

  // virtual std::variant<Image<uint8_t>, Image<uint16_t>> read_full_image() = 0;
  // virtual std::variant<Image<uint8_t>, Image<uint16_t>> read_image_patch(	std::pair<std::size_t, std::size_t> origin,
  // 																std::pair<std::size_t, std::size_t> length) = 0;
  virtual std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_full_image() = 0;
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PIXELMAPFILEBINARY_H__ */