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

/** \file     ImageColorSpacesConversor.h
 *  \brief    Converts between color spaces.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-28
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGECOLORSPACESCONVERSOR_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGECOLORSPACESCONVERSOR_H__

#include <functional>  //for std::function
#include <type_traits>
#include <typeinfo>
#include <variant>
#include "ColorSpaces.h"
#include "Image.h"


namespace ImageColorSpaceConversion {

template<template<typename> class ImageT, typename T>
constexpr bool is_ycbcr() {
  return std::is_base_of_v<YCbCrImage<T>, ImageT<T>>;
}

template<template<typename> class ImageT, typename T>
constexpr bool is_rgb() {
  return std::is_base_of_v<RGBImage<T>, ImageT<T>>;
}

template<template<typename> class ImageOut, template<typename> class ImageIn,
    typename T>
ImageOut<T> ycbcr_conversion(const ImageIn<T>& source) {
  static_assert(std::is_base_of_v<Image<T>, ImageIn<T>>,
      "In RGB/YCbCr conversion, input must be an Image...");
  static_assert(std::is_base_of_v<Image<T>, ImageOut<T>>,
      "In RGB/YCbCr conversion, output must be an Image...");

  //checking correct usage
  if constexpr (is_ycbcr<ImageIn, T>()) {
    static_assert(
        is_rgb<ImageOut, T>(), "As input is YCbCr, output must be RGB");

  } else {
    static_assert(is_rgb<ImageIn, T>() || std::is_same_v<Image<T>, ImageIn<T>>,
        "As input is not YCbCr, input must be RGB or Base Image...");
    if constexpr (!is_rgb<ImageIn, T>()) {
      if (source.get_type() != ImageType::RGB) {
        std::cerr << "Input is a base image with a type different from RGB."
                  << std::endl;
        exit(2);
      }
    }
    static_assert(
        is_ycbcr<ImageOut, T>(), "As input is not YCbCr, output must be...");
  }

  using namespace ColorSpaces;

  auto converted_image =
      ImageOut<T>(source.get_width(), source.get_height(), source.get_bpp());

  auto a_data = source[0].data();
  auto b_data = source[1].data();
  auto c_data = source[2].data();

  auto d_data = converted_image[0].data();
  auto e_data = converted_image[1].data();
  auto f_data = converted_image[2].data();

  constexpr auto keep_dynamic_range = true;

  typedef
      typename std::conditional<std::is_base_of_v<YCbCrImage<T>, ImageIn<T>>,
          ImageIn<T>, ImageOut<T>>::type YCbCrImageT;

  typedef typename std::conditional<
      std::is_same_v<YCbCrImageT, BT601Image<T>>,  //condition
      BT601Coefficients,  //if true
      typename std::conditional<  //if false
          std::is_same_v<YCbCrImageT, BT709Image<T>>,  //condition
          BT709Coefficients,  // if true
          BT2020Coefficients  //if false
          >::type>::type
      color_conversion_coefficients;  //could generate error if not bt 2020


  auto converter_provider =
      ConversorProvider<T, color_conversion_coefficients, keep_dynamic_range>();

  auto convert = converter_provider.getConverter(source.get_bpp());

  std::function<std::tuple<T, T, T>(
      decltype(convert.get()), const std::tuple<T, T, T>&)>
      conversion_function;

  if constexpr (std::is_same<RGBImage<T>, ImageOut<T>>::value) {
    conversion_function = [](decltype(convert.get()) convert,
                              const std::tuple<T, T, T>& source_pel) {
      return convert->to_RGB(source_pel);
    };
  } else {
    conversion_function = [](decltype(convert.get()) convert,
                              const std::tuple<T, T, T>& source_pel) {
      return convert->to_YCbCr(source_pel);
    };
  }

  for (decltype(source.get_number_of_pixels_per_channel()) i = 0;
       i < source.get_number_of_pixels_per_channel(); ++i) {
    auto sourcel_pel = std::make_tuple(*a_data++, *b_data++, *c_data++);
    std::tie(*d_data++, *e_data++, *f_data++) = conversion_function(convert.get(), sourcel_pel);
  }

  return converted_image;
}

namespace convert {

template<template<typename> class ImageOut, template<typename> class ImageIn,
    typename T>
ImageOut<T> to(const ImageIn<T>& source) {
  static_assert(std::is_base_of<Image<T>, ImageIn<T>>::value,
      "The input for conversion must be an image...");
  static_assert(std::is_base_of<Image<T>, ImageOut<T>>::value,
      "The output for conversion must be an image...");


  if constexpr (std::is_same<ImageIn<T>, ImageOut<T>>::value) {
    return source;
  }

  if constexpr (is_ycbcr<ImageIn, T>() && is_ycbcr<ImageOut, T>()) {
    auto temp_as_rgb = to<RGBImage>(source);
    return to<ImageOut>(temp_as_rgb);
  }

  if constexpr ((is_rgb<ImageIn, T>() && is_ycbcr<ImageOut, T>()) ||
                (is_rgb<ImageOut, T>() && is_ycbcr<ImageIn, T>())) {
    return ycbcr_conversion<ImageOut>(source);
  }

  switch (source.get_type()) {
    case ImageType::RGB: {
      if constexpr (std::is_base_of_v<YCbCrImage<T>, ImageOut<T>> &&
                    !std::is_same_v<YCbCrImage<T>, ImageOut<T>>) {
        //since the output is YCbCr, I can derive the coefficients..
        //however, what if is YCbCr (not any BTXXX)??
        if constexpr (!std::is_base_of_v<YCbCrImage<T>,
                          ImageIn<T>>)  //Im not yet sure why this is needed...
          return ycbcr_conversion<ImageOut>(
              source);  //static_cast_ptr<RGBImage<T>, Image<T>>(source)
      }
      break;
    }

    default:
      std::cerr << "Conversion from this type is not yet supported..."
                << std::endl;
      exit(2);
  }

  return ImageOut<T>(
      source.get_width(), source.get_height(), source.get_bpp());
}

template<template<typename> class ImageOut, template<typename> class ImageIn,
    typename T>
std::unique_ptr<ImageOut<T>> to(const std::unique_ptr<ImageIn<T>>& source) {
  auto out_image = to<ImageOut>(*(source.get()));
  return std::make_unique<ImageOut<T>>(std::move(out_image));
}

template<template<typename> class ImageOut, template<typename> class ImageIn,
    typename T1, typename T2>
std::variant<std::unique_ptr<ImageOut<T1>>, std::unique_ptr<ImageOut<T2>>> to(
    const std::variant<std::unique_ptr<ImageIn<T1>>,
        std::unique_ptr<ImageIn<T2>>>& source) {
  if (source.index() == 0) {
    return to<ImageOut>(std::get<0>(source));
  }
  return to<ImageOut>(std::get<1>(source));
}

}  // namespace convert

}  // namespace ImageColorSpaceConversion

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGECOLORSPACESCONVERSOR_H__ */