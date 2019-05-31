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
#include "ImageChannel.h"

enum ImageType { Invalid, GrayScale, RGB, YCoCg, BT601, BT709, BT2020 };

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


  Image(const Image<T>& other) : type(other.type) {
    channels = other.channels;
  }


  Image(Image<T>&& other) noexcept : type(other.type) {
    *this = std::move(other);
  }

  auto clone() const {
    return std::unique_ptr<Image<T>>(generate_ptr_to_clone()) ;
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


  decltype(channels.size()) get_number_of_channels() const {
    return channels.size();
  }


  std::size_t get_width() const {
    return channels[0].get_width();
  }


  std::size_t get_height() const {
    return channels[0].get_height();
  }


  std::size_t get_bpp() const {
    return channels[0].get_bpp();
  }


  std::size_t get_number_of_pixels_per_channel() const {
    return channels[0].get_number_of_pixels();
  }


  std::size_t get_number_of_pixels() const {
    return (this->get_number_of_channels()) *
           (this->get_number_of_pixels_per_channel());
  }


  decltype(type) get_type() const {
    return type;
  }


  void set_pixel_at(T value, std::size_t channel, size_t i, size_t j) {
    channels.at(channel).set_value_at(i, j, value);
  }


  T get_pixel_at(std::size_t channel, size_t i, size_t j) const {
    return channels.at(channel).get_value_at(i, j);
  }


  ImageChannel<T>& operator[](int i) {
    return channels[i];
  }


  const ImageChannel<T>& operator[](int i) const {
    return channels[i];
  }


  ImageChannel<T>& get_channel(int i) {
    return channels[i];
  }


  std::vector<ImageChannel<T>>& get_channels() {
    return channels;
  }


  const ImageChannel<T>& get_channel(int i) const {
    return channels[i];
  }

  void shift_pixels_by(int8_t shift) {
    for (auto& channel : channels) {
      channel.shift_pixels_by(shift);
    }
  }


  virtual std::vector<std::string> get_channel_names() const = 0;
};


template<typename T>
class ThreeChannelImage : public Image<T> {
 public:
  ThreeChannelImage(
      std::size_t width, std::size_t height, std::size_t bpp, ImageType type)
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

  std::tuple<T, T, T> get_pixel_at(std::size_t i, std::size_t j) const {
    return std::make_tuple(this->channels[0].get_pixel_at(i, j),
        this->channels[1].get_pixel_at(i, j),
        this->channels[2].get_pixel_at(i, j));
  }

  std::tuple<T, T, T> get_pixel_at(
      std::pair<std::size_t, std::size_t> coordinate) const {
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
    set_value_at(pixel, std::get<0>(coordinate), std::get<1>(coordinate));
  }

  virtual ~ThreeChannelImage() = default;
};


template<typename T>
class RGBImage : public ThreeChannelImage<T> {
 public:
  RGBImage(std::size_t width, std::size_t height, std::size_t bpp)
      : ThreeChannelImage<T>(width, height, bpp, ImageType::RGB){};


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


template<typename T>
class YCbCrImage : public ThreeChannelImage<T> {
 public:
  YCbCrImage(
      std::size_t width, std::size_t height, std::size_t bpp, ImageType type)
      : ThreeChannelImage<T>(width, height, bpp, type){};


  YCbCrImage(YCbCrImage<T>&& other) noexcept
      : ThreeChannelImage<T>(std::move(other)) {
  }


  YCbCrImage(const YCbCrImage<T>& other) : ThreeChannelImage<T>(other){};


  YCbCrImage& operator=(YCbCrImage<T>&& other) {
    ThreeChannelImage<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const YCbCrImage<T>& other) const {
    return this->is_equal(other);
  }

  ~YCbCrImage() = default;


  std::vector<std::string> get_channel_names() const final {
    return {"Y", "Cb", "Cr"};
  }
};


template<typename T>
class BT601Image : public YCbCrImage<T> {
 public:
  BT601Image(std::size_t width, std::size_t height, std::size_t bpp)
      : YCbCrImage<T>(width, height, bpp, ImageType::BT601){};

  BT601Image(BT601Image<T>&& other) noexcept : YCbCrImage<T>(std::move(other)) {
  }


  BT601Image(const BT601Image<T>& other) : YCbCrImage<T>(other){};


  BT601Image& operator=(BT601Image<T>&& other) {
    YCbCrImage<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const BT601Image<T>& other) const {
    return this->is_equal(other);
  }


  ~BT601Image() = default;


  virtual BT601Image* generate_ptr_to_clone() const override {
    return new BT601Image<T>(*this);
  }
};

template<typename T>
class BT709Image : public YCbCrImage<T> {
 public:
  BT709Image(std::size_t width, std::size_t height, std::size_t bpp)
      : YCbCrImage<T>(width, height, bpp, ImageType::BT709){};

  BT709Image(BT709Image<T>&& other) noexcept : YCbCrImage<T>(std::move(other)) {
  }


  BT709Image(const BT709Image<T>& other) : YCbCrImage<T>(other){};


  BT709Image& operator=(BT709Image<T>&& other) {
    YCbCrImage<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const BT709Image<T>& other) const {
    return this->is_equal(other);
  }


  ~BT709Image() = default;


  virtual BT709Image* generate_ptr_to_clone() const override {
    return new BT709Image<T>(*this);
  }
};

template<typename T>
class BT2020Image : public YCbCrImage<T> {
 public:
  BT2020Image(std::size_t width, std::size_t height, std::size_t bpp)
      : YCbCrImage<T>(width, height, bpp, ImageType::BT2020){};


  BT2020Image(BT2020Image<T>&& other) noexcept
      : YCbCrImage<T>(std::move(other)) {
  }


  BT2020Image(const BT2020Image<T>& other) : YCbCrImage<T>(other){};


  BT2020Image& operator=(BT2020Image<T>&& other) {
    YCbCrImage<T>::operator=(std::move(other));
    return *this;
  }


  inline bool operator==(const BT2020Image<T>& other) const {
    return this->is_equal(other);
  }

  ~BT2020Image() = default;


  virtual BT2020Image* generate_ptr_to_clone() const override {
    return new BT2020Image<T>(*this);
  }
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