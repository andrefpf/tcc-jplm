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

#ifndef JPLM_LIB_UTILS_IMAGE_RGBIMAGE_H__
#define JPLM_LIB_UTILS_IMAGE_RGBIMAGE_H__

#include "Lib/Utils/Image/ThreeChannelImage.h"

template<typename T>
class RGBImage : public ThreeChannelImage<T> {
 public:
  static constexpr auto image_type = ImageType::RGB;
  RGBImage(std::size_t width, std::size_t height, std::size_t bpp)
      : ThreeChannelImage<T>(width, height, bpp, image_type){};


  RGBImage(RGBImage<T>&& other) noexcept
      : ThreeChannelImage<T>(std::move(other)) {
  }


  RGBImage(const RGBImage<T>& other) : ThreeChannelImage<T>(other){};


  RGBImage& operator=(RGBImage<T>&& other) {
    ThreeChannelImage<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const RGBImage<T>& other) const {
    return this->is_equal(other);
  }


  inline bool operator!=(const RGBImage<T>& other) const {
    return !this->is_equal(other);
  }


  virtual RGBImage* generate_ptr_to_clone() const override {
    return new RGBImage<T>(*this);
  }


  ~RGBImage() = default;


  std::vector<std::string> get_channel_names() const final {
    return {"Red", "Green", "Blue"};
  }

};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_RGBIMAGE_H__ */