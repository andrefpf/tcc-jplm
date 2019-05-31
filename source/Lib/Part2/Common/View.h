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
 private:
  std::unique_ptr<Image<T>> image;

 public:
  static_assert(std::is_integral<T>::value, "The view type must be integral");
  View(std::unique_ptr<Image<T>> image) : image(std::move(image)){};
  View() {
  	std::cout << "This is the view default constructor..." << std::endl;
  }
   //for now this is needed in the lightfield constructor

  View(const View<T>& other) : image(other.image->clone()) {
  }

  View(View<T>&& other) noexcept {
  	std::cout << "Im in the view move constructor" << std::endl;
    *this = std::move(other);
  }


  View<T>& operator=(View<T>&& other) noexcept {
    if (this != &other) {
      // std::cout << "Im in the view move assign" << std::endl;
      // std::swap(this->image, other.image);
      // other.image.release();
      this->image = std::move(other.image);
    }
    return *this;
  }

  void operator=(const View<T>& other) {
  	std::cout << "Im in copy assign" << std::endl;
  	this->image = other.image->clone();
  	// if (!this->image) {
  	// } else {
  	// 	this->image.reset();

  	// 	// (other.image->clone());
  	// }
  }

  ~View() = default;

  decltype(image->get_width()) get_width() const noexcept {
  	if(!image) {
  		return 0;	
  	}
    return image->get_width();
  }

  decltype(image->get_height()) get_height() const noexcept {
  	if(!image) {
  		return 0;	
  	}
    return image->get_height();
  }

  decltype(image->get_bpp()) get_bpp() const noexcept {
  	if(!image) {
  		return 0;	
  	}
    return image->get_bpp();
  }

  ImageType get_image_type() const noexcept {
  	if(!image) {
  		return ImageType::Invalid;	
  	}
    return image->get_type();
  }
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEW_H__ */