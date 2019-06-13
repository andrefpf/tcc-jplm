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

/** \file     PixelMapFile.h
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-28
 */

#ifndef JPLM_LIB_UTILS_IMAGE_PIXELMAPFILE_H__
#define JPLM_LIB_UTILS_IMAGE_PIXELMAPFILE_H__

#include "Lib/Utils/Image/ImageFile.h"


//P1 and P4 are bitmaps, PBM (portable bitmap), or pbm - Netpbm bi-level image format according to http://netpbm.sourceforge.net/doc/pbm.html
//P2 and P5 are grayscale images, PGM (portable graymap)
//P3 and P6 are fullcolor images, PPM (portable pixmap)
enum PixelMapType { Unknown = 0, P1, P2, P3, P4, P5, P6 };

class PixelMapFile : public ImageFile {
 protected:
  std::string filename;
  std::fstream file;
  std::streampos raster_begin;  // should be const..
  PixelMapType type = PixelMapType::Unknown;
  std::size_t width = 0;
  std::size_t height = 0;
  std::size_t max_value = 0;

 public:
  PixelMapFile(const std::string& file_name, PixelMapType type)
      : filename(file_name), type(type){};


  PixelMapFile(std::filesystem::path& file_name, PixelMapType type)
      : filename(file_name.c_str()), type(type){};


  PixelMapFile(const PixelMapFile& other) = delete;


  PixelMapFile(PixelMapFile&& other) {
    std::swap(filename, other.filename);
    std::swap(type, other.type);
    std::swap(width, other.width);
    std::swap(height, other.height);
    std::swap(max_value, other.max_value);
    std::swap(file, other.file);
    std::swap(raster_begin, other.raster_begin);
  }


  PixelMapFile(const std::string& file_name, PixelMapType type,
      std::size_t width, std::size_t height, std::size_t max_value)
      : filename(file_name), type(type), width(width), height(height),
        max_value(max_value){};


  PixelMapFile(const std::string& file_name, const std::streampos raster_begin,
      PixelMapType type, std::size_t width, std::size_t height,
      std::size_t max_value)
      : filename(file_name), raster_begin(raster_begin), type(type),
        width(width), height(height), max_value(max_value){};

  ~PixelMapFile();

  void open();

  decltype(type) get_type() const noexcept {
    return type;
  }


  decltype(raster_begin) get_raster_begin() const {
    return raster_begin;
  }


  decltype(width) get_width() const noexcept {
    return width;
  }


  decltype(height) get_height() const noexcept {
    return height;
  }


  bool has_properties(PixelMapType type, std::size_t width, std::size_t height,
      std::size_t max_value) const noexcept {
    if (this->width != width)
      return false;
    if (this->height != height)
      return false;
    if (this->max_value != max_value)
      return false;
    if (this->type != type)
      return false;

    return true;
  }

  bool is_equivalent_to(const PixelMapFile& other) const noexcept {
    return has_properties(
        other.type, other.width, other.height, other.max_value);
  };

  std::size_t get_number_of_bits_per_pixel() const {
    std::size_t bits = 1;
    while (std::pow(2, bits) < max_value) {
      ++bits;
    }
    return bits;
  }

  virtual std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_full_image() = 0;
  virtual std::variant<std::unique_ptr<Image<uint8_t>>,
      std::unique_ptr<Image<uint16_t>>>
  read_image_patch(std::pair<std::size_t, std::size_t> origin,
      std::pair<std::size_t, std::size_t> lenght) = 0;
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PIXELMAPFILE_H__ */