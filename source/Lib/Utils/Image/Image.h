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

  /**
   * \brief      Determines this Image is equal to other Image with same type.
   *
   * \param[in]  other  The other
   *
   * \return     True if equal, False otherwise.
   */
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


  /**
   * \brief      Copy constructor of the Image
   * \details    Copies the type and then uses the std::vector copy constructor to copy channels
   *
   * \param[in]  other  The other
   */
  Image(const Image<T>& other) : type(other.type) {
    channels = other.channels;
  }


  /**
   * \brief      Move constructor of the Image
   * \details    Copies the other type and then uses move assigment
   * 
   * \param      other  The other
   */
  Image(Image<T>&& other) noexcept : type(other.type) {
    *this = std::move(other);
  }


  /**
   * \brief      Creates a new instance of the object with same properties than original.
   * \details    Provides a deep copy of the image (derived classes included)
   *
   * \return     Copy of this object.
   */
  auto clone() const {
    return std::unique_ptr<Image<T>>(generate_ptr_to_clone());
  }


  virtual ~Image() = default;


  /**
   * \brief      Pure virtual function used to make a deep copy. 
   *
   * \return     A pointer to the derived image (as implemented in derive classes)
   */
  virtual Image* generate_ptr_to_clone() const = 0;


  /**
   * \brief      Image move assignment.
   *
   * \param      other  The other image
   *
   * \return     This image with the data from other
   */
  Image& operator=(Image&& other) {
    if (this != &other) {
      this->channels.clear();
      std::swap(channels, other.channels);
    }
    return *this;
  }


  /**
   * \brief      Image copy assignment.
   *
   * \param[in]  other  The other image
   *
   * \return     This image with a copy of the data from the other image.
   */
  Image& operator=(const Image& other) {
    if (this != &other) {
      this->channels = other.channels;
    }
    return *this;
  }


  /**
   * \brief      Equal operator
   *
   * \param[in]  other  The other image to be compared with this
   *
   * \return     If the other image is equal this image, returns true. Otherwise, returns false.
   */
  bool operator==(const Image<T>& other) const {
    return this->is_equal(other);
  }


  /**
   * \brief      Not equal operator
   *
   * \param[in]  other  The other image to be compared with this
   *
   * \return     If the other image is equal this image, returns false. Otherwise, returns true.
   * 
   * \details It is the oposite of operator==. Thus, uses operator== to avoid replicating code.
   */
  bool operator!=(const Image<T>& other) const {
    return !(*this == other);
  }


  /**
   * \brief      Gets the number of channels.
   *
   * \return     The number of channels that this image have.
   */
  auto get_number_of_channels() const {
    return channels.size();
  }


  /**
   * \brief      Gets the width of this Image.
   *
   * \return     The width.
   */
  auto get_width() const {
    return channels[0].get_width();
  }


  /**
   * \brief      Gets the height of this Image.
   *
   * \return     The height.
   */
  auto get_height() const {
    return channels[0].get_height();
  }


  /**
   * \brief      Gets the bits per pixel (bpp) of this Image.
   *
   * \return     The bits per pixel.
   */
  auto get_bpp() const {
    return channels[0].get_bpp();
  }


  /**
   * \brief      Gets a vector with the bits per pixel (bpp) of each and every channel of this Image.
   * \details    Useful for images with different bpp according to channel
   *
   * \return     The bpps.
   */
  auto get_bpps() const {
    std::vector<std::size_t> bpps;
    bpps.reserve(channels.size());
    for (const auto& channel : channels) {
      bpps.emplace_back(channel.get_bpp());
    }
    return bpps;
  }

  /**
   * \brief      Gets the number of pixels per channel of this Image.
   *
   * \return     The number of pixels per channel.
   */
  auto get_number_of_pixels_per_channel() const {
    return channels[0].get_number_of_pixels();
  }


  /**
   * \brief      Gets a vector with the umber of pixels of each and every channel of this Image.
   * \details    Useful for images with different number of pixels according to channel
   *
   * \return     The std::vector with number of pixels.
   */
  auto get_number_of_pixels_of_each_channel() const {
    std::vector<std::size_t> number_of_pixels;
    number_of_pixels.reserve(channels.size());
    for (const auto& channel : channels) {
      number_of_pixels.emplace_back(channel.get_number_of_pixels());
    }
    return number_of_pixels;
  }


  /**
   * \brief      Gets the number of pixels of this Image.
   *
   * \return     The number of pixels.
   */
  auto get_number_of_pixels() const {
    return (this->get_number_of_channels()) *
           (this->get_number_of_pixels_per_channel());
  }


  /**
   * \brief      Gets the type of this Image.
   *
   * \return     The type, from ImageType enum.
   */
  auto get_type() const {
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