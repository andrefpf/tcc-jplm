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

/** \file     Lightfield.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#ifndef JPLM_LIB_PART2_COMMON_LIGHTFIELD_H__
#define JPLM_LIB_PART2_COMMON_LIGHTFIELD_H__

#include "Lib/Part2/Common/LightfieldCoordinate.h"
#include "Lib/Part2/Common/LightfieldDimension.h"
#include "Lib/Part2/Common/View.h"
#include "Lib/Part2/Common/ViewIOPolicies.h"
#include "Lib/Utils/Image/Generic2DStructure.h"

//template<class ViewT<typename>,


template<typename T>
class Lightfield : public Generic2DStructure<std::unique_ptr<View<T>>> {
 protected:
  std::unique_ptr<ViewIOPolicy<T>> view_io_policy;

 public:
  Lightfield(const std::size_t width, const std::size_t height,
      const bool auto_alloc_resources = true)
      : Generic2DStructure<std::unique_ptr<View<T>>>(
            width, height, auto_alloc_resources),
        view_io_policy(std::make_unique<ViewIOPolicyLimitlessMemory<T>>()){};

  Lightfield(const std::pair<std::size_t, std::size_t>& t_s_size,
      const bool auto_alloc_resources = true)
      : Lightfield(std::get<0>(t_s_size), std::get<1>(t_s_size),
            auto_alloc_resources) {
  }

  
  Lightfield(const Lightfield& other) : Generic2DStructure<std::unique_ptr<View<T>>>(other) {
    this->view_io_policy = std::unique_ptr<ViewIOPolicy<T>>(other.view_io_policy->clone());
    //the copy of content should be handled by generic2dStructure...
  };


  ~Lightfield() = default;


  void set_view_io_policy(std::unique_ptr<ViewIOPolicy<T>>&& view_io_policy) {
    this->view_io_policy = std::move(view_io_policy);
  }


  virtual T get_value_at(const std::size_t channel,
      const LightfieldCoordinate<std::size_t>& coordinate) const {
    auto& view = get_view_at(coordinate.get_t_and_s());
    return view_io_policy->get_value_at(
        view, channel, coordinate.get_v_and_u());
  }


  virtual View<T>& get_view_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    auto& view_unique_ptr_ref = this->get_element_reference_at(coordinate);
    return *(view_unique_ptr_ref.get());
  }


  //getters with move and copy may cause object slicing
  virtual std::unique_ptr<View<T>> get_view_copy_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    return std::move(this->get_element_reference_at(coordinate));
  }


  virtual const Image<T>& get_image_at(const std::pair<std::size_t, std::size_t>& coordinate) const {
    auto& view = get_view_at(coordinate);
    return view_io_policy->get_image_at(view);
  }


  template<typename ViewType = View<T>>
  void set_view_at(const ViewType& view,
      const std::pair<std::size_t, std::size_t>& coordinate) {
    this->set_element_at(
        std::move(std::make_unique<ViewType>(view)), coordinate);
  }


  template<typename ViewType = View<T>>
  void set_view_at(const ViewType&& view,
      const std::pair<std::size_t, std::size_t>& coordinate) {
    this->set_element_at(
        std::move(std::make_unique<ViewType>(std::move(view))), coordinate);
  }


  auto get_views_width() const {
    if (!this->elements) {
      //Throws
    }
    return this->elements[0]->get_width();
  }


  auto get_views_height() const {
    if (!this->elements) {
      //Throws
    }
    return this->elements[0]->get_height();
  }


  auto get_views_bpp() const {
    if (!this->elements) {
      //Throws
    }
    return this->elements[0]->get_bpp();
  }


  auto get_dimensions() const {
    return LightfieldDimension(this->get_width(), this->get_height(),
        this->get_views_width(), this->get_views_height());
  }


  inline View<T>* operator[](const int i) {
    // auto unique_ptr_to_view = *this->elements_for_2d_access[i];
    // return unique_ptr_to_view.get();
    return (*this->elements_for_2d_access[i]).get();
  }


  inline const View<T>* operator[](const int i) const {
    // auto unique_ptr_to_view = *this->elements_for_2d_access[i];
    return (*this->elements_for_2d_access[i]).get();
  }


  virtual Lightfield* clone() const override {
    return new Lightfield(*this);
  };
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELD_H__ */