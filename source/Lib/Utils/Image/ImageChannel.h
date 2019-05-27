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
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>
#include "ImageExceptions.h"
#include "Metrics.h"

template<typename T>
class ImageChannel {
 private:
  const std::size_t width;
  const std::size_t height;
  const std::size_t bpp;
  const std::size_t number_of_pixels = 0;
  std::unique_ptr<T[]> pixels;
  std::vector<T*> pixels_2d;


  void alloc_resources() {
    // number_of_pixels = width * height;
    if (!pixels) {
      pixels = std::make_unique<T[]>(number_of_pixels);
    }
    if (pixels_2d.size() != 0)
      pixels_2d.clear();

    pixels_2d.resize(height);
    for (auto i = decltype(height){0}; i < height; ++i) {
      pixels_2d[i] = &pixels[i * width];
    }
  }


  inline bool is_value_valid(const T value) {
    if constexpr (std::numeric_limits<T>::is_signed) {
      return (get_min_value() <= value && value <= get_max_value());
    }
    return value <= get_max_value();
  }


 public:
  ImageChannel(const std::size_t width, const std::size_t height, const std::size_t bpp)
      : width(width), height(height), bpp(bpp),
        number_of_pixels(width * height),
        pixels(std::make_unique<T[]>(number_of_pixels)) {
    if (number_of_pixels == 0 || bpp == 0)
      throw ImageChannelExceptions::InvalidSizeException();
    alloc_resources();
  }


  ImageChannel(const ImageChannel<T>& other) noexcept
      : width(other.width), height(other.height), bpp(other.bpp),
        number_of_pixels(other.number_of_pixels) {
    alloc_resources();
    std::memcpy(pixels.get(), other.pixels.get(), number_of_pixels * sizeof(T));
  }


  ImageChannel(ImageChannel<T>&& other) noexcept
      : width(other.width), height(other.height), bpp(other.bpp),
        number_of_pixels(other.number_of_pixels) {
    *this = std::move(other);
  }


  ImageChannel<T>& operator=(ImageChannel<T>&& other) noexcept {
    if (this != &other) {
      pixels = std::move(other.pixels);
      std::swap(pixels_2d, other.pixels_2d);
    }
    return *this;
  }


  ~ImageChannel() = default;


  void fill_with(const T value) {
    if constexpr (std::is_same<T, uint8_t>::value) {
      std::memset(pixels.get(), value, number_of_pixels);
    } else {
      for (auto i = decltype(number_of_pixels){0}; i < number_of_pixels; ++i) {
        pixels[i] = value;
      }
    }
  }


  inline bool is_index_valid(const std::pair<std::size_t, std::size_t> index) const {
    auto [i, j] = index;
    return i < width && j < height;
  }


  inline bool is_index_valid(const std::size_t i, std::size_t j) const {
    return is_index_valid({i, j});
  }


  void set_value_at(const T value, const std::pair<std::size_t, std::size_t> coordinate) {
    const auto& [i, j] = coordinate;
    set_value_at(value, i, j);
  }


  void set_value_at(const T value, const std::size_t i, const std::size_t j) {
    if (!is_index_valid(i, j)) {
      throw ImageChannelExceptions::InvalidIndexWriteException();
    }
    if (!is_value_valid(value)) {
      throw ImageChannelExceptions::InvalidValueException();
    }
    pixels[i * width + j] = value;
  }


  T get_value_at(const std::pair<std::size_t, std::size_t> coordinate) const {
    const auto& [i, j] = coordinate;
    return get_value_at(i, j);
  }


  T get_value_at(const std::size_t i, std::size_t j) const {
    if (!is_index_valid(i, j)) {
      throw ImageChannelExceptions::InvalidIndexReadException();
    }
    if (this->pixels != nullptr) {
      return pixels[i * width + j];
    } else {
      std::cerr << "ptr not set.. " << std::endl;
      return 0;
    }
  }


  inline T* data() const {
    return pixels.get();
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
    auto temp_ptr = pixels.get();
    auto ret_vector = std::vector<T>();
    ret_vector.reserve(number_of_pixels);
    for (auto i = decltype(number_of_pixels){0}; i < number_of_pixels; ++i) {
      ret_vector.emplace_back(*temp_ptr);
      ++temp_ptr;
    }
    return ret_vector;
  }


  inline T* operator[](const int i) {
    return pixels_2d[i];
  }


  inline const T* operator[](const int i) const {
    return pixels_2d[i];
  }


  bool has_equal_size(const ImageChannel<T>& other) const noexcept {
    return (other.width == width) && (other.height == height);
  }


  bool operator==(const ImageChannel<T>& other) const {
    if (!has_equal_size(other))
      return false;
    for (auto i = decltype(number_of_pixels){0}; i < number_of_pixels; ++i) {
      if (pixels[i] != other.data()[i])
        return false;
    }
    return true;
  }


  void operator=(const ImageChannel<T>& other) {
    if (!has_equal_size(other)) {  //needs to realloc
      alloc_resources();
    }
    std::memcpy(pixels.get(), other.pixels.get(), number_of_pixels * sizeof(T));
  }


  typedef typename std::make_signed<T>::type signed_type;


  ImageChannel<signed_type> operator-(const ImageChannel<T>& other) const {
    auto max_bpp = std::max(bpp, other.bpp);
    auto difference_image = ImageChannel<signed_type>(
        width, height, max_bpp + 1);  //needs one bit more than the inputs

    auto this_ptr = pixels.get();
    auto other_ptr = other.data();
    auto diff_ptr = difference_image.data();
    for (std::size_t i = 0; i < number_of_pixels; ++i) {
      *diff_ptr = static_cast<signed_type>(*this_ptr - *other_ptr);
      ++diff_ptr;
      ++this_ptr;
      ++other_ptr;
    }

    return difference_image;
  }


  inline decltype(width) get_width() const noexcept {
    return width;
  }


  inline decltype(height) get_height() const noexcept {
    return height;
  }


  inline auto get_dimension() const noexcept {
    return std::make_pair(width, height);
  }


  inline decltype(bpp) get_bpp() const noexcept {
    return bpp;
  }


  inline decltype(number_of_pixels) get_number_of_pixels() const noexcept {
    return number_of_pixels;
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGECHANNEL_H__ */
