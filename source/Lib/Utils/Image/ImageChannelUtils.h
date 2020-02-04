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

/** \file     ImageChannelUtils.h
 *  \brief    Test of image utils.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-20
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGECHANNELUTILS_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGECHANNELUTILS_H__


#include "Lib/Utils/Image/ImageChannel.h"
#include "Lib/Utils/Image/Metrics.h"

namespace ImageChannelUtils {

template<typename T>
ImageChannel<typename std::make_unsigned<T>::type> absolute(
    const ImageChannel<T>& image_channel) {
  if constexpr (std::is_unsigned<T>())
    return image_channel;

  auto absolute_image = ImageChannel<typename std::make_unsigned<T>::type>(
      image_channel.width, image_channel.height, image_channel.max_bpp - 1);
  auto image_channel_ptr = image_channel.data();
  auto absolute_image_ptr = absolute_image.data();

  for (std::size_t i = 0; i < image_channel.getnumber_of_pixels; ++i) {
    *absolute_image_ptr = std::abs(*image_channel_ptr);
    absolute_image_ptr++;
    image_channel_ptr++;
  }
}

template<typename T>
double get_sum_of_squared_errors(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {
  auto differences_image = original_channel - encoded_channel;
  auto error_vector = differences_image.as_raster_vector();
  return Metrics::get_sum_of_squared_errors<typename std::make_signed<T>::type>(
      error_vector);
}

template<typename T>
double get_mse(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {
  auto sse = get_sum_of_squared_errors<T>(original_channel, encoded_channel);
  auto size = original_channel.get_number_of_pixels();
  return sse / static_cast<double>(size);
}

template<typename T>
double get_peak_signal_to_noise_ratio(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {
  auto max_value =
      std::pow(2.0, static_cast<double>(original_channel.get_bpp())) - 1;
  return 10.0 * std::log10((max_value * max_value) /
                           get_mse(original_channel, encoded_channel));
}


template<typename T>
double get_maximum_absolute_error(const ImageChannel<T>& original_channel,
    const ImageChannel<T>& encoded_channel) {

  auto number_of_pixels = original_channel.get_number_of_pixels();

  auto differences_image = original_channel - encoded_channel;
  auto error_vector = differences_image.as_raster_vector();
  return Metrics::get_maximum_absolute_error<typename std::make_signed<T>::type>(
       error_vector);

  // std::cout << std::dec;
  // for(auto i=decltype(number_of_pixels){0};i<number_of_pixels;++i) {
  //   std::cout << original_channel[i] << std::endl;
  //   std::cout << encoded_channel[i] << std::endl;
  //   std::cout << original_channel[i] - encoded_channel[i] << std::endl;
  //   auto error = std::abs(original_channel[i] - encoded_channel[i]);
  //   std::cout << "error " << error << std::endl;
  //   if(error > maximum_error) {
  //     maximum_error = error;
  //   }
  // }

  // return maximum_error;

}

}  // namespace ImageChannelUtils

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGECHANNELUTILS_H__ */