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

/** \file     UndefinedImage.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-12-11
 */

#ifndef JPLM_LIB_UTILS_IMAGE_UNDEFINEDIMAGE_H__
#define JPLM_LIB_UTILS_IMAGE_UNDEFINEDIMAGE_H__

#include "Lib/Utils/Image/Image.h"

template<typename T>
class UndefinedImage : public Image<T> {
 public:
  static constexpr auto image_type = ImageType::Undefined;


  UndefinedImage(std::size_t width, std::size_t height, std::size_t bpp,
      std::size_t number_of_channels = 1)
      : Image<T>(width, height, bpp, number_of_channels, image_type){};


  UndefinedImage(UndefinedImage<T>&& other) noexcept
      : Image<T>(std::move(other)) {
  }


  UndefinedImage(const UndefinedImage<T>& other) : Image<T>(other){};


  UndefinedImage& operator=(UndefinedImage<T>&& other) {
    Image<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const UndefinedImage<T>& other) const {
    return this->is_equal(other);
  }


  inline bool operator!=(const UndefinedImage<T>& other) const {
    return !this->is_equal(other);
  }


  virtual UndefinedImage* generate_ptr_to_clone() const override {
    return new UndefinedImage<T>(*this);
  }


  virtual ~UndefinedImage() = default;


  virtual std::vector<std::string> get_channel_names() const override {
    return std::vector<std::string>(this->get_number_of_channels(), "Unnamed");
  }

  // void merge(const UndefinedImage& other) {

  // }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_UNDEFINEDIMAGE_H__ */