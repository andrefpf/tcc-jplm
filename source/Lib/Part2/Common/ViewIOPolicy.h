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

/** \file     ViewIOPolicy.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-06-06
 */

#ifndef JPLM_LIB_PART2_COMMON_VIEWIOPOLICY_H__
#define JPLM_LIB_PART2_COMMON_VIEWIOPOLICY_H__


#include "Lib/Part2/Common/CommonExceptions.h"
#include "Lib/Part2/Common/View.h"
#include "Lib/Utils/Image/Image.h"
#include "Lib/Utils/Image/ImageColorSpacesConversor.h"

template<typename T>
class ViewIOPolicy {
 protected:
  virtual void load_image_if_necessary(View<T>& view) = 0;
  bool save_image_when_release = false;
  bool overwrite_image_when_save_if_file_already_exists = false;

 public:
  ViewIOPolicy() = default;


  ViewIOPolicy(const ViewIOPolicy<T>& other) = default;


  ViewIOPolicy(ViewIOPolicy<T>&& other)
      : save_image_when_release(other.save_image_when_release),
        overwrite_image_when_save_if_file_already_exists(
            other.overwrite_image_when_save_if_file_already_exists) {
  }


  virtual ~ViewIOPolicy() = default;


  void set_save_image_when_release(bool save) {
    save_image_when_release = save;
  }


  void set_overwrite_image_when_save_if_file_already_exists(bool overwrite) {
    overwrite_image_when_save_if_file_already_exists = overwrite;
  }


  virtual T get_value_at(View<T>& view, const std::size_t channel,
      const std::pair<std::size_t, std::size_t>& coordinate) {
    load_image_if_necessary(view);
    return view.get_value_at(channel, coordinate);
  }


  virtual Image<T>& get_image_at(View<T>& view) {
    load_image_if_necessary(view);
    return *view.get_image_ptr();
  }


  template<template<typename> class ImageType>
  ImageType<T>& get_image_at(View<T>& view) {
    load_image_if_necessary(view);
    using namespace ImageColorSpaceConversion;
    auto image_ptr = view.get_image_ptr();
    if (const auto image_ptr = view.get_image_ptr();
        image_ptr->get_type() == ImageType<T>::image_type) {
      return static_cast<ImageType<T>&>(*image_ptr);
    }
    auto view_image = view.release_image();
    auto converted_image = convert::to<ImageType>(view_image);
    view.set_image(std::move(converted_image));

    return static_cast<ImageType<T>&>(*view.get_image_ptr());
  }


  void release_image_from_view(View<T>& view) {
    if(save_image_when_release) {
      view.write_image(overwrite_image_when_save_if_file_already_exists);
    }
    auto view_image = view.release_image();
  }


  virtual ViewIOPolicy<T>* clone() const = 0;
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_VIEWIOPOLICY_H__ */