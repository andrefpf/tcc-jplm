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

/** \file     ImageFile.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-08
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGEFILE_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGEFILE_H__

#include <inttypes.h>
#include <cmath>
#include <experimental/tuple>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

class ImageFile {
 protected:
  std::string filename;
  std::fstream file;
  std::streampos raster_begin;  // should be const..
  std::size_t width = 0;
  std::size_t height = 0;


 public:
  ImageFile(const std::string& file_name) : filename(file_name){};


  ImageFile(const std::filesystem::path& file_name) : filename(file_name.c_str()){};


  ImageFile(const ImageFile& other) = delete;


  ImageFile(ImageFile&& other) {
    std::swap(filename, other.filename);
    std::swap(width, other.width);
    std::swap(height, other.height);
    std::swap(file, other.file);
    std::swap(raster_begin, other.raster_begin);
  }


  ImageFile(const std::string& file_name, std::size_t width, std::size_t height)
      : filename(file_name), width(width), height(height){};


  ImageFile(const std::string& file_name, const std::streampos raster_begin,
      std::size_t width, std::size_t height)
      : filename(file_name), raster_begin(raster_begin), width(width),
        height(height){};


  virtual ~ImageFile();

  auto get_width() const noexcept {
    return width;
  }


  auto get_height() const noexcept {
    return height;
  }


  decltype(raster_begin) get_raster_begin() const {
    return raster_begin;
  }

  void open();
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEFILE_H__ */