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

/** \file     ImageChannel.h
 *  \brief    Template class that describes an image channel
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-20
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGECHANNEL_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGECHANNEL_H__

#include <inttypes.h>
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>
#include "Generic2DStructure.h"
#include "ImageExceptions.h"
#include "Metrics.h"

template<typename T>
class ImageChannel : public Generic2DStructure<T> {
 private:
  const std::size_t bpp;

 protected:
  inline bool is_value_valid(const T value) {
    if constexpr (std::numeric_limits<T>::is_signed) {
      return (get_min_value() <= value && value <= get_max_value());
    }
    return value <= get_max_value();
  }


 public:
  ImageChannel(
      const std::size_t width, const std::size_t height, const std::size_t bpp)
      : Generic2DStructure<T>(width, height, false), bpp(bpp) {
    if (bpp == 0)
      throw ImageChannelExceptions::InvalidSizeException();
    this->alloc_all_resources();
  }


  ImageChannel(const ImageChannel<T>& other) noexcept
      : Generic2DStructure<T>(other.width, other.height), bpp(other.bpp) {
    std::memcpy(this->elements.get(), other.elements.get(),
        this->number_of_elements * sizeof(T));
  }


  ImageChannel(ImageChannel<T>&& other) noexcept
      : Generic2DStructure<T>(other.width, other.height), bpp(other.bpp) {
        //TODO: check if it is possible to initiallyze the Generic2DStructure with false
    *this = std::move(other);
  }


  ImageChannel<T>& operator=(ImageChannel<T>&& other) noexcept {
    if (this != &other) {
      this->elements = std::move(other.elements);
      std::swap(this->elements_for_2d_access, other.elements_for_2d_access);
    }
    return *this;
  }


  ~ImageChannel() = default;


  virtual ImageChannel* generate_ptr_to_clone() const override {
    return new ImageChannel(*this);
  }


  void fill_with(const T value) {
    if constexpr (std::is_same<T, uint8_t>::value) {
      std::memset(this->elements.get(), value, this->number_of_elements);
    } else {
      for (auto i = decltype(this->number_of_elements){0};
           i < this->number_of_elements; ++i) {
        this->elements[i] = value;
      }
    }
  }


  inline T get_max_value() const {
    auto positive_bpp = bpp;
    if constexpr (std::numeric_limits<T>::is_signed) {
      --positive_bpp;
    }
    return std::pow(2.0, static_cast<double>(positive_bpp)) - 1;
  }


  inline T get_min_value() const {
    if constexpr (std::numeric_limits<T>::is_signed) {
      return -std::pow(2.0, static_cast<double>(bpp - 1));
    }
    return 0;
  }


  std::vector<T> as_raster_vector() {
    auto temp_ptr = this->elements.get();
    auto ret_vector = std::vector<T>();
    ret_vector.reserve(this->number_of_elements);
    for (auto i = decltype(this->number_of_elements){0};
         i < this->number_of_elements; ++i) {
      ret_vector.emplace_back(*temp_ptr);
      ++temp_ptr;
    }
    return ret_vector;
  }


  inline T* operator[](const int i) {
    return this->elements_for_2d_access[i];
  }


  inline const T* operator[](const int i) const {
    return this->elements_for_2d_access[i];
  }


  bool operator==(const ImageChannel<T>& other) const {
    if (!this->has_equal_size(other))
      return false;
    for (auto i = decltype(this->number_of_elements){0};
         i < this->number_of_elements; ++i) {
      if (this->elements[i] != other.data()[i])
        return false;
    }
    return true;
  }


  void operator=(const ImageChannel<T>& other) {
    if (!this->has_equal_size(other)) {  //needs to realloc
      this->alloc_all_resources();
    }
    std::memcpy(this->elements.get(), other.elements.get(),
        this->number_of_elements * sizeof(T));
  }


  typedef typename std::make_signed<T>::type signed_type;


  ImageChannel<signed_type> operator-(const ImageChannel<T>& other) const {
    auto max_bpp = std::max(bpp, other.bpp);
    auto difference_image = ImageChannel<signed_type>(this->width, this->height,
        max_bpp + 1);  //needs one bit more than the inputs

    auto this_ptr = this->elements.get();
    auto other_ptr = other.data();
    auto diff_ptr = difference_image.data();
    for (std::size_t i = 0; i < this->number_of_elements; ++i) {
      *diff_ptr = static_cast<signed_type>(*this_ptr - *other_ptr);
      ++diff_ptr;
      ++this_ptr;
      ++other_ptr;
    }

    return difference_image;
  }


  void set_value_at(const T& value, const std::size_t i, const std::size_t j) {
    if (!is_value_valid(value)) {
      throw ImageChannelExceptions::InvalidValueException();
    }
    Generic2DStructure<T>::set_element_at(value, i, j);
  }


  void set_value_at(
      const T& value, const std::pair<std::size_t, std::size_t>& coordinate) {
    if (!is_value_valid(value)) {
      throw ImageChannelExceptions::InvalidValueException();
    }
    Generic2DStructure<T>::set_element_at(
        value, std::get<0>(coordinate), std::get<1>(coordinate));
  }


  T get_value_at(const std::pair<std::size_t, std::size_t>& coordinate) const {
    // std::cout << "getting pixel in channel" << std::endl;
    return this->get_element_value_at(coordinate);
  }


  T get_value_at(const std::size_t i, const std::size_t j) const {
    return this->get_element_value_at(i, j);
  }


  inline auto get_bpp() const noexcept {
    return bpp;
  }


  inline auto get_number_of_pixels() const noexcept {
    return this->number_of_elements;
  }


  void shift_pixels_by(int8_t shift) {
    if (shift == 0)
      return;

    std::function<T(T)> used_shift;
    if (shift < 0) {  //i.e, negative
      shift *= -1;
      used_shift = [this, shift](T value) -> T {
        if constexpr (std::is_integral<T>::value) {
          return value >> shift;
        }
        return value;  //FIXME
      };
    } else {
      used_shift = [this, shift](T value) -> T {
        if constexpr (std::is_integral<T>::value) {
          auto shifted = value << shift;
          auto max = this->get_max_value();
          if ((shifted > max) || (shifted < value))  //overflow
            return max;
          return shifted;
        }
        return value;  //FIXME
      };
    }

    std::transform(this->begin(), this->end(), this->begin(), used_shift);
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGECHANNEL_H__ */
