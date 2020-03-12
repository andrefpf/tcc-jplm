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
#include "Lib/Utils/Image/UndefinedImage.h"

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

  return output_image;
}


template<template<typename> class ImageOut, typename Tout>
std::unique_ptr<ImageOut<Tout>> get_undefined_images_as(
    const std::vector<std::unique_ptr<UndefinedImage<Tout>>>& images) {
  auto width = images[0]->get_width();
  auto height = images[0]->get_height();
  auto output_image =
      std::make_unique<ImageOut<Tout>>(width, height, images[0]->get_bpp());

  auto number_of_channels = output_image->get_number_of_channels();
  if (number_of_channels != images.size()) {
    //! \todo throw exception if the number of channels in the returned image is different from the number of input images
    //throw expression
    std::cout << "Should throw" << std::endl;
  }
  for (auto i = decltype(number_of_channels){0}; i < number_of_channels; ++i) {
    (*output_image)[i] = (*(images[i]))[0];
  }

  return output_image;
}


template<typename Tout>
std::unique_ptr<UndefinedImage<Tout>> get_undefined_images_as_undefined_image(
    const std::vector<std::unique_ptr<UndefinedImage<Tout>>>& images) {
  auto width = images[0]->get_width();
  auto height = images[0]->get_height();

  auto output_image = std::make_unique<UndefinedImage<Tout>>(
      width, height, images[0]->get_bpp(), images.size());

  auto number_of_channels = output_image->get_number_of_channels();
  if (number_of_channels != images.size()) {
    //! \todo throw exception if the number of channels in the returned image is different from the number of input images
    //throw expression
    std::cout << "Should throw" << std::endl;
  }
  for (auto i = decltype(number_of_channels){0}; i < number_of_channels; ++i) {
    (*output_image)[i] = (*(images[i]))[0];
  }

  return output_image;
}


//! \todo make this (get_undefined_images_as) variadic
template<template<typename> class ImageOut, typename Tout>
std::unique_ptr<ImageOut<Tout>> get_undefined_images_as(
    const UndefinedImage<Tout>& channel_0,
    const UndefinedImage<Tout>& channel_1,
    const UndefinedImage<Tout>& channel_2) {
  auto width = channel_0.get_width();
  auto height = channel_0.get_height();
  auto output_image =
      std::make_unique<ImageOut<Tout>>(width, height, channel_0.get_bpp());
  //std::size_t width, std::size_t height, std::size_t bpp//, std::size_t number_of_channels)

  for (auto i = decltype(height){0}; i < height; ++i) {
    for (auto j = decltype(width){0}; j < width; ++j) {
      output_image->set_pixel_at(
          {channel_0.get_value_at(0, i, j), channel_1.get_value_at(0, i, j),
              channel_2.get_value_at(0, i, j)},
          {i, j});
    }
  }

  return output_image;
}


//! \todo implement a get_splitting_of from ImageIn<Tin>&& or std::unique_ptr<ImageIn<Tin>>&&
template<template<typename> class ImageIn, typename Tin>
std::vector<std::unique_ptr<UndefinedImage<Tin>>> get_splitting_of(
    const ImageIn<Tin>& input_image) {
  auto width = input_image.get_width();
  auto height = input_image.get_height();
  auto bpp = input_image.get_bpp();

  auto output_images = std::vector<std::unique_ptr<UndefinedImage<Tin>>>();
  output_images.reserve(input_image.get_number_of_channels());

  for (const auto& channel : input_image) {
    auto image = std::make_unique<UndefinedImage<Tin>>(width, height, bpp, 1);
    (*image)[0] = channel;
    output_images.push_back(std::move(image));
  }

  return output_images;
}


}  // namespace ImageUtils

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGEUTILS_H__ */