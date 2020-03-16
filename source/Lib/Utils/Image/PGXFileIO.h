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

/** \file     PGXFileIO.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-05
 */

#ifndef JPLM_LIB_UTILS_IMAGE_PGXFILEIO_H__
#define JPLM_LIB_UTILS_IMAGE_PGXFILEIO_H__

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include "Lib/Utils/Image/ImageExceptions.h"
#include "Lib/Utils/Image/PGXFile.h"


namespace PGXFileIO {

template<typename T>
struct UndefinedImageVisitor {
  template<typename T0>
  std::unique_ptr<UndefinedImage<T>> operator()(
      std::unique_ptr<UndefinedImage<T0>>& image) {
    if constexpr (std::is_same<T0, T>::value) {
      return std::unique_ptr<UndefinedImage<T>>(
          static_cast<UndefinedImage<T>*>(image.release()));
    }
    auto output_image = std::make_unique<UndefinedImage<T>>(
        image->get_width(), image->get_height(), 1, image->get_bpp());
    auto output_image_iterator = output_image->begin();

    for (const auto& channel : *(image)) {
      auto output_image_channel_iterator = output_image_iterator->begin();
      for (const auto value : channel) {
        *output_image_channel_iterator = static_cast<T>(value);
        ++output_image_channel_iterator;
      }
      ++output_image_iterator;
    }

    return output_image;
  }
};


std::unique_ptr<PGXFile> open(const std::string& filename);


std::unique_ptr<PGXFile> open(const std::string& filename, std::size_t width,
    std::size_t height, std::size_t depth, bool is_signed);

}  // namespace PGXFileIO

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PGXFILEIO_H__ */