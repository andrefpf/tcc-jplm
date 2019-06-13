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

/** \file     Image.h
 *  \brief    Image class.
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-04-29
 */

#ifndef JPLM_LIB_UTILS_IMAGE_IMAGE_H__
#define JPLM_LIB_UTILS_IMAGE_IMAGE_H__

#include <vector>
#include "Lib/Utils/Image/ImageChannel.h"

//The BT types should also support full and reduced dynamic ranges
enum ImageType { Invalid, GrayScale, RGB, BT601, BT709, BT2020, YCoCg };


template<typename T>
class Image {
 protected:
  const ImageType type;
  std::vector<ImageChannel<T>> channels;


  bool is_equal(const Image<T>& other) const {
    if (this != &other) {
      if (this->get_number_of_channels() != other.get_number_of_channels()) {
        return false;
      }
      for (auto i = decltype(get_number_of_channels()){0};
           i < this->get_number_of_channels(); ++i) {
        if (!(channels[i] == other.get_channel(i))) {
          return false;
        }
      }
    }
    return true;
  }


 public:
  Image(std::size_t width, std::size_t height, std::size_t bpp,
      std::size_t number_of_channels, ImageType type)
      : type(type) {
    channels.reserve(number_of_channels);
    for (decltype(number_of_channels) i = 0; i < number_of_channels; ++i) {
      channels.emplace_back(ImageChannel<T>(width, height, bpp));
    }
  }


  //this constructor allows for creating image channels with different bpps
  Image(std::size_t width, std::size_t height, std::vector<std::size_t> bpps,
      ImageType type)
      : type(type) {
    channels.reserve(bpps.size());
    for (const auto& bpp : bpps) {
      channels.emplace_back(ImageChannel<T>(width, height, bpp));
    }
  }


  Image(const Image<T>& other) : type(other.type) {
    channels = other.channels;
  }


  Image(Image<T>&& other) noexcept : type(other.type) {
    *this = std::move(other);
  }

  auto clone() const {
    return std::unique_ptr<Image<T>>(generate_ptr_to_clone());
  }


  virtual ~Image() = default;


  virtual Image* generate_ptr_to_clone() const = 0;


  Image& operator=(Image&& other) {
    if (this != &other) {
      this->channels.clear();
      std::swap(channels, other.channels);
    }
    return *this;
  }


  Image& operator=(const Image& other) {
    if (this != &other) {
      this->channels = other.channels;
    }
    return *this;
  }


  bool operator==(const Image<T>& other) const {
    return this->is_equal(other);
  }


  bool operator!=(const Image<T>& other) const {
    return !(*this == other);
  }


  auto get_number_of_channels() const {
    return channels.size();
  }


  auto get_width() const {
    return channels[0].get_width();
  }


  auto get_height() const {
    return channels[0].get_height();
  }


  auto get_bpp() const {
    return channels[0].get_bpp();
  }


  auto get_bpps() const {
    std::vector<std::size_t> bpps;
    bpps.reserve(channels.size());
    for (const auto& channel : channels) {
      bpps.emplace_back(channel.get_bpp());
    }
    return bpps;
  }


  auto get_number_of_pixels_per_channel() const {
    return channels[0].get_number_of_pixels();
  }


  auto get_number_of_pixels() const {
    return (this->get_number_of_channels()) *
           (this->get_number_of_pixels_per_channel());
  }


  decltype(type) get_type() const {
    return type;
  }


  void set_pixel_at(T value, const std::size_t channel, const std::size_t i,
      const std::size_t j) {
    channels.at(channel).set_value_at(i, j, value);
  }


  T get_pixel_at(const std::size_t channel, const std::size_t i,
      const std::size_t j) const {
    return this->get_value_at(channel, i, j);
  }


  T get_pixel_at(const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    return this->get_value_at(channel, coordinate);
  }


  T get_value_at(const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    return channels.at(channel).get_value_at(coordinate);
  }


  T get_value_at(const std::size_t channel, const std::size_t i,
      const std::size_t j) const {
    return channels.at(channel).get_value_at(i, j);
  }


  inline ImageChannel<T>& operator[](const int i) {
    return channels[i];
  }


  inline const ImageChannel<T>& operator[](const int i) const {
    return channels[i];
  }


  ImageChannel<T>& get_channel(const int i) {
    return channels[i];
  }


  std::vector<ImageChannel<T>>& get_channels() {
    return channels;
  }


  const ImageChannel<T>& get_channel(const int i) const {
    return channels[i];
  }

  void shift_pixels_by(int8_t shift) {
    for (auto& channel : channels) {
      channel.shift_pixels_by(shift);
    }
  }


  auto begin() {
    return channels.begin();
  }


  auto end() {
    return channels.end();
  }


  auto begin() const noexcept {
    return this->channels.cbegin();
  }


  auto end() const noexcept {
    return this->channels.cend();
  }


  auto cbegin() const noexcept {
    return channels.cbegin();
  }


  auto cend() const noexcept {
    return channels.cend();
  }


  virtual std::vector<std::string> get_channel_names() const = 0;
};




template<typename T>
class GrayScaleImage : public Image<T> {
 public:
  GrayScaleImage(std::size_t width, std::size_t height, std::size_t bpp)
      : Image<T>(width, height, bpp, 1, ImageType::GrayScale){};


  GrayScaleImage(GrayScaleImage<T>&& other) noexcept
      : Image<T>(std::move(other)) {
  }


  GrayScaleImage(const GrayScaleImage<T>& other) : Image<T>(other){};


  GrayScaleImage& operator=(GrayScaleImage<T>&& other) {
    Image<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const GrayScaleImage<T>& other) const {
    return this->is_equal(other);
  }


  inline bool operator!=(const GrayScaleImage<T>& other) const {
    return !this->is_equal(other);
  }


  ~GrayScaleImage() = default;


  std::vector<std::string> get_channel_names() const final {
    return {"Gray"};
  }


  virtual GrayScaleImage* generate_ptr_to_clone() const override {
    return new GrayScaleImage<T>(*this);
  }
};


#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_IMAGE_H__ */