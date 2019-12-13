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

#ifndef JPLM_LIB_UTILS_IMAGE_THREECHANNELIMAGE_H__
#define JPLM_LIB_UTILS_IMAGE_THREECHANNELIMAGE_H__

#include "Lib/Utils/Image/Image.h"

template<typename T>
class ThreeChannelImage : public Image<T> {
 public:
  ThreeChannelImage(const std::size_t width, const std::size_t height,
      const std::size_t bpp, const ImageType type)
      : Image<T>(width, height, bpp, 3, type){};


  ThreeChannelImage(ThreeChannelImage<T>&& other) noexcept
      : Image<T>(std::move(other)) {
  }


  ThreeChannelImage(const ThreeChannelImage<T>& other) : Image<T>(other){};


  ThreeChannelImage& operator=(ThreeChannelImage<T>&& other) {
    Image<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const ThreeChannelImage<T>& other) const {
    return this->is_equal(other);
  }


  inline bool operator!=(const ThreeChannelImage<T>& other) const {
    return !this->is_equal(other);
  }


  std::tuple<T, T, T> get_pixel_at(
      const std::size_t i, const std::size_t j) const {
    return std::make_tuple(this->channels[0].get_value_at(i, j),
        this->channels[1].get_value_at(i, j),
        this->channels[2].get_value_at(i, j));
  }


  std::tuple<T, T, T> get_pixel_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    return get_pixel_at(std::get<0>(coordinate), std::get<1>(coordinate));
  }


  void set_pixel_at(
      const std::tuple<T, T, T>& pixel, std::size_t i, std::size_t j) {
    this->channels[0].set_value_at(std::get<0>(pixel), i, j);
    this->channels[1].set_value_at(std::get<1>(pixel), i, j);
    this->channels[2].set_value_at(std::get<2>(pixel), i, j);
  }


  void set_pixel_at(const std::tuple<T, T, T>& pixel,
      std::pair<std::size_t, std::size_t> coordinate) {
    set_pixel_at(pixel, std::get<0>(coordinate), std::get<1>(coordinate));
  }


  virtual ~ThreeChannelImage() = default;
};


#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_THREECHANNELIMAGE_H__ */