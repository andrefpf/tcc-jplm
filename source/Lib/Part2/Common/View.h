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

#include <Image.h>
#include <type_traits>  //is_integral

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


  View(View<T>&& other) noexcept {
    // std::cout << "Im in the view move constructor" << std::endl;
    *this = std::move(other);
  }


  void operator=(View<T>&& other) noexcept {
    // std::cout << "Im in the view move assign" << std::endl;
    if (this != &other) {
      std::swap(this->image_, other.image_);
      std::swap(this->view_size, other.view_size);
      std::swap(this->bpp, other.bpp);
    }
  }


  void operator=(Image<T>&& source_image) noexcept {
    std::cout << "using this weird operator" << std::endl;
    std::swap(this->image_, source_image);
  }


  void operator=(const View<T>& other) {
    // std::cout << "Im in view copy assign" << std::endl;
    this->image_ = other.image_->clone();
  }


  ~View() = default;


  decltype(image_->get_width()) get_width() const noexcept {
    if (!image_) {
      return 0;
    }
    return image_->get_width();
  }


  decltype(image_->get_height()) get_height() const noexcept {
    if (!image_) {
      return 0;
    }
    return image_->get_height();
  }


  decltype(image_->get_bpp()) get_bpp() const noexcept {
    if (!image_) {
      return 0;
    }
    return image_->get_bpp();
  }


  ImageType get_image_type() const noexcept {
    if (!image_) {
      return ImageType::Invalid;
    }
    return image_->get_type();
  }


  inline ImageChannel<T>& operator[](const int i) {
    // auto& ret_val = (*(image_.get()))[i];
    return (*(image_.get()))[i];
  }


  inline const ImageChannel<T>& operator[](const int i) const {
    return (*(image_.get()))[i];
  }


  std::tuple<T, T, T> get_pixel_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    if (image_->get_number_of_channels() != 3) {
      //THROW
    }
    auto image_with_three_channels =
        static_cast<ThreeChannelImage<T>*>(image_.get());
    return image_with_three_channels->get_pixel_at(coordinate);
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEW_H__ */