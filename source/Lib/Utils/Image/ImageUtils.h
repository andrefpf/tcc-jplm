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

/** \file     ThreeChannelImage.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-13
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGEUTILS_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGEUTILS_H__

#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageColorSpacesConversor.h"

namespace ImageUtils {

template<typename Tout, typename Tin>
constexpr void check_image_container_requisites(const Image<Tin>& image_in) {
  if constexpr (std::is_same<Tin, Tout>::value) {
    return;  //if the types are the same, there is no need for further checking
  }
  if (image_in.get_bpp() > sizeof(Tout) * 8) {
    throw ImageUtilsExceptions::ContainerHasFewerBitsThanNeededException();
  }
  if constexpr (std::is_signed<Tin>::value && std::is_unsigned<Tout>::value) {
    //check in to garantee all values >= 0;
    for (const auto& channel : image_in) {
      for (const auto& value : channel) {
        if constexpr (std::is_signed<Tin>::value &&
                      std::is_unsigned<Tout>::value) {
          if (value < 0) {
            throw ImageUtilsExceptions::
                NegativeValueInUnsignedVariableException();
          }
        }
      }
    }
  }
}


// template<class ImageClass, typename Tout, typename Tin>
template<template<typename> class ImageTout, template<typename> class ImageTin,
    typename Tout, typename Tin>
std::unique_ptr<ImageTout<Tout>> get_image_with_new_container_type(
    const std::unique_ptr<ImageTin<Tin>>& image_in) {
  check_image_container_requisites<Tout, Tin>(*image_in.get());
  // auto image_out = std::make_unique<ImageTout<Tout>>(image_in->get_width(), image_in->get_height(), image_in->get_bpp());
  auto intermediary_image =
      ImageColorSpaceConversion::convert::to<ImageTout>(image_in);
  if constexpr (std::is_same<Tin, Tout>::value) {
    return intermediary_image;
  }
  auto output_image = std::make_unique<ImageTout<Tout>>(
      image_in->get_width(), image_in->get_height(), image_in->get_bpp());
  auto output_image_iterator = output_image->begin();

  for (const auto& channel : *intermediary_image) {
    auto output_image_channel_iterator = output_image_iterator->begin();
    for (const auto& value : channel) {
      *output_image_channel_iterator = static_cast<Tout>(value);
      output_image_channel_iterator++;
    }
    output_image_iterator++;
  }

  return std::move(output_image);
}


}  // namespace ImageUtils

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEUTILS_H__ */