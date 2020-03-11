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

/** \file     PixelMapFileIO.h
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-07
 */

#ifndef JPLM_LIB_UTILS_IMAGE_PIXELMAPFILEIO_H__
#define JPLM_LIB_UTILS_IMAGE_PIXELMAPFILEIO_H__

#include <memory>
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageExceptions.h"
#include "Lib/Utils/Image/ImageUtils.h"
#include "Lib/Utils/Image/PPMBinaryFile.h"
#include "Lib/Utils/Image/PixelMapFile.h"


namespace PixelMapFileIO {

std::unique_ptr<PixelMapFile> open(const std::string& filename);
std::unique_ptr<PixelMapFile> open(const std::string& filename,
    PixelMapType type, std::size_t width, std::size_t height,
    std::size_t max_value);

#ifdef _WIN32
std::uint16_t count_line_breaks(std::string filename, std::streamoff end);
#endif

struct VariantImageExtractorToUint16T {
  template<template<typename> class ImageT0, typename T0>
  std::unique_ptr<RGBImage<uint16_t>> operator()(
      std::unique_ptr<ImageT0<T0>>& image) {
    if constexpr (std::is_same<T0, uint16_t>::value) {
      return std::unique_ptr<RGBImage<uint16_t>>(
          static_cast<RGBImage<uint16_t>*>(image.release()));
    }

    auto output_image = std::make_unique<RGBImage<uint16_t>>(
        image->get_width(), image->get_height(), image->get_bpp());
    auto output_image_iterator = output_image->begin();

    for (const auto& channel : *(image)) {
      auto output_image_channel_iterator = output_image_iterator->begin();
      for (const auto value : channel) {
        *output_image_channel_iterator = static_cast<uint16_t>(value);
        ++output_image_channel_iterator;
      }
      ++output_image_iterator;
    }

    return output_image;
  }
};


template<template<typename> class ImageTout, typename Tout>
std::unique_ptr<ImageTout<Tout>> extract_image_with_type_from_variant(
    std::variant<std::unique_ptr<Image<uint8_t>>,
        std::unique_ptr<Image<uint16_t>>>& input) {
  auto image_uint16_t = std::visit(VariantImageExtractorToUint16T(), input);
  auto ret_image = ImageUtils::get_image_with_new_container_type<ImageTout,
      RGBImage, Tout, uint16_t>(std::move(image_uint16_t));
  return ret_image;
}

}  // namespace PixelMapFileIO


#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_PIXELMAPFILEIO_H__ */