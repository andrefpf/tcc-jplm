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

/** \file     View.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEW_H__
#define JPLM_LIB_PART2_COMMON_VIEW_H__

#include <type_traits>  //is_integral
#include "Lib/Part2/Common/CommonExceptions.h"
#include "Lib/Utils/Image/Image.h"

template<typename T>
class View {
 protected:
  std::unique_ptr<Image<T>> image_;
  std::pair<std::size_t, std::size_t> view_size;
  std::size_t bpp;

 public:
  static_assert(std::is_integral<T>::value, "The view type must be integral");


  View(std::unique_ptr<Image<T>> image)
      : image_(std::move(image)),
        view_size(image_->get_width(), image_->get_height()),
        bpp(image_->get_bpp()){};

  View(const std::pair<std::size_t, std::size_t>& view_size,
      const std::size_t bpp)
      : image_(nullptr), view_size(view_size), bpp(bpp) {
    if (bpp == 0) {
      throw ViewExceptions::InvalidZeroBppException();
    }
    if (this->get_width() == 0) {
      throw ViewExceptions::InvalidZeroWidthException();
    }
    if (this->get_height() == 0) {
      throw ViewExceptions::InvalidZeroHeightException();
    }
  }


  View() {
    // std::cout << "This is the view default constructor..." << std::endl;
  }
  //for now this is needed in the lightfield constructor


  View(const View<T>& other)
      : image_(other.get_image_clone()), view_size(other.view_size),
        bpp(other.bpp) {
  }


  std::unique_ptr<Image<T>> get_image_clone() const noexcept {
    if (!image_) {
      return nullptr;
    }
    return image_->clone();
  }


  Image<T>* get_image_ptr() {
    if (!image_) {
      return nullptr;
    }
    return image_.get();
  }

  void release_image() {
    image_.reset(nullptr);
  }


  void set_image(Image<T>&& image) {
    image_ = std::move(image);
  }


  View(View<T>&& other) noexcept {
    *this = std::move(other);
  }


  void operator=(View<T>&& other) noexcept {
    if (this != &other) {
      std::swap(this->image_, other.image_);
      std::swap(this->view_size, other.view_size);
      std::swap(this->bpp, other.bpp);
    }
  }


  void operator=(Image<T>&& source_image) noexcept {
    std::swap(this->image_, source_image);
    this->view_size = std::make_pair(image_->get_width(), image_->get_height()),
    this->bpp = image_->get_bpp();
    if (this->bpp == 0) {
      throw ViewExceptions::InvalidZeroBppException();
    }
  }


  void operator=(const View<T>& other) {
    this->image_ = other.get_image_clone();
    this->view_size = other.view_size;
    this->bpp = other.bpp;
  }


  virtual ~View() = default;


  auto get_width() const noexcept {
    return std::get<0>(view_size);
  }


  auto get_height() const noexcept {
    return std::get<1>(view_size);
  }


  auto get_bpp() const noexcept {
    return bpp;
  }


  ImageType get_image_type() const noexcept {
    if (!image_) {
      return ImageType::Invalid;
    }
    return image_->get_type();
  }


  auto get_number_of_channels() const {
    if (!image_) {
      throw ViewExceptions::ImageWasNotInitialyzedException();
    }
    return image_->get_number_of_channels();
  }


  //these are unsafe
  inline virtual ImageChannel<T>& operator[](const int i) noexcept {
    return (*(image_.get()))[i];
  }


  inline virtual const ImageChannel<T>& operator[](const int i) const noexcept {
    return (*(image_.get()))[i];
  }


  virtual std::tuple<T, T, T> get_pixel_at(
      const std::pair<std::size_t, std::size_t>& coordinate) {
    if (!image_) {
      load_image(view_size);
    }
    if (!image_) {
      throw ViewExceptions::ImageWasNotInitialyzedException();
    }
    if (image_->get_number_of_channels() != 3) {
      throw ViewExceptions::InvalidNumberOfChannelsException();
    }
    auto image_with_three_channels =
        static_cast<ThreeChannelImage<T>*>(image_.get());
    return image_with_three_channels->get_pixel_at(coordinate);
  }


  virtual T get_value_at(const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) {
    if (!image_) {
      load_image(view_size);
    }
    if (!image_) {
      throw ViewExceptions::ImageWasNotInitialyzedException();
    }
    if (channel >= image_->get_number_of_channels()) {
      throw ViewExceptions::InvalidNumberOfChannelsException();
    }
    return image_->get_pixel_at(channel, coordinate);
  }


  virtual void load_image(const std::pair<std::size_t, std::size_t>& size,
      const std::pair<std::size_t, std::size_t>& initial = {0, 0}) {
    std::cout << "Im not sure how to load a image with size "
              << std::get<0>(size) << "x" << std::get<1>(size)
              << "with initial point at (" << std::get<0>(initial) << ", "
              << std::get<1>(initial) << ")" << std::endl;
  }

  void load_image() {
    load_image(this->view_size);
  }
};


#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEW_H__ */