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
#include "Lib/Part2/Common/ViewIOPolicy.h"
#include "Lib/Part2/Common/ViewIOPolicyLimitlessMemory.h"
#include "Lib/Utils/Image/Generic2DStructure.h"

//template<class ViewT<typename>,

enum class LightFieldDimensions {
  T = 0,
  S = 1,
  V = 2,
  U = 3,
};

template<typename T>
class Lightfield : public Generic2DStructure<std::unique_ptr<View<T>>> {
 protected:
  std::unique_ptr<ViewIOPolicy<T>> view_io_policy;
  mutable std::unique_ptr<LightfieldDimension<std::size_t>> lightfield_dimension =
      nullptr;

 public:
  /**
   * \brief      Constructor of the Lightfield object.
   *
   * \param[in]  width                 The width (number of horizontal views of the Lightfield)
   * \param[in]  height                The height (number of vertical views of the Lightfield)
   * \param[in]  view_io_policy        The view i/o policy
   * \param[in]  auto_alloc_resources  If set, automatic allocate the resources of the Generic2DStructure.
   * 
   * Instantiates a new Lightfield according to the parameters, using a view_io_policy
   * 
   */
  Lightfield(const std::size_t width, const std::size_t height,
      // ViewIOPolicy<T>&& view_io_policy = ViewIOPolicyLimitlessMemory<T>(),
      const ViewIOPolicy<T>& view_io_policy = ViewIOPolicyLimitlessMemory<T>(),
      const bool auto_alloc_resources = true)
      : Generic2DStructure<std::unique_ptr<View<T>>>(
            width, height, auto_alloc_resources),
        view_io_policy(
            std::unique_ptr<ViewIOPolicy<T>>(view_io_policy.clone())) {
  }


  /**
   * \brief      Constructor of the Lightfield object using a pair.
   *
   * \param[in]  t_s_size              A std::pair containing the width and height (t, s) of the Lightfield
   * \param[in]  view_io_policy        The view i/o policy
   * \param[in]  auto_alloc_resources  The automatic allocate resources
   * 
   * \details    Delegates the construction of the object to the 
   */
  Lightfield(const std::pair<std::size_t, std::size_t>& t_s_size,
      const ViewIOPolicy<T>& view_io_policy = ViewIOPolicyLimitlessMemory<T>(),
      const bool auto_alloc_resources = true)
      : Lightfield(std::get<0>(t_s_size), std::get<1>(t_s_size), view_io_policy,
            auto_alloc_resources) {
  }


  /**
   * \brief      Copy constructor of the Lightfield object.
   *
   * \param[in]  other  The lightfield that is the source for this copy
   */
  Lightfield(const Lightfield& other)
      : Generic2DStructure<std::unique_ptr<View<T>>>(other) {
    this->view_io_policy =
        std::unique_ptr<ViewIOPolicy<T>>(other.view_io_policy->clone());
    //the copy of content should be handled by generic2dStructure...
  };


  /**
   * \brief      Destroys the Lightfield using the default destructor.
   * \details    Notice that no freeing of resources is necessary, as it is
   * being taken care by std::unique_ptr
   * 
   */
  virtual ~Lightfield() = default;


  /**
   * \brief      Sets the view i/o policy.
   *
   * \param[in]      view_io_policy  An r-value reference to the view i/o policy
   * 
   * \details    Transfers the ownership of the view_io_policy to this lightfield. 
   * 
   * \e Example: 
   * \snippet App/Utils/LightfieldVisualization.cpp Setting a view_io_policy into a Lightfield
   */
  void set_view_io_policy(std::unique_ptr<ViewIOPolicy<T>>&& view_io_policy) {
    this->view_io_policy = std::move(view_io_policy);
  }


  /**
   * \brief Sets the view i/o policy.
   * \details Creates a deep copy of a view_io_policy which will be owned by this lightfield
   * 
   * \param[in] view_io_policy Const reference to a view_io_policy
   */
  void set_view_io_policy(const ViewIOPolicy<T>& view_io_policy) {
    this->view_io_policy =
        std::unique_ptr<ViewIOPolicy<T>>(view_io_policy->clone());
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


  virtual const Image<T>& get_image_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    auto& view = get_view_at(coordinate);
    return view_io_policy->get_image_at(view);
  }


  virtual Image<T>& get_image_at(
      const std::pair<std::size_t, std::size_t>& coordinate) {
    auto& view = get_view_at(coordinate);
    return view_io_policy->get_image_at(view);
  }


  template<template<typename> typename ImageType>
  const ImageType<T>& get_image_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    auto& view = get_view_at(coordinate);
    return view_io_policy->template get_image_at<ImageType>(view);
  }
  

  template<template<typename> typename ImageType>
  ImageType<T>& get_image_at(
      const std::pair<std::size_t, std::size_t>& coordinate) {
    auto& view = get_view_at(coordinate);
    return view_io_policy->template get_image_at<ImageType>(view);
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
    this->set_element_at(std::make_unique<ViewType>(std::move(view)), coordinate);
  }


  template<typename ViewType = View<T>>
  void set_view_at(std::unique_ptr<ViewType>&& view, const std::pair<std::size_t, std::size_t>& coordinate) {
    this->set_element_at(std::move(view), coordinate);
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


  auto get_number_of_channels_in_view() const {
    if (!this->elements) {
      //Throws
    }
    return this->elements[0]->get_number_of_channels();
  }


  auto get_number_of_pixels_per_view_channel() const {
    if (!this->elements) {
      //Throws
    }
    return get_views_width() * get_views_height();
  }


  auto get_views_bpp() const {
    if (!this->elements) {
      //Throws
    }
    return this->elements[0]->get_bpp();
  }


  template<typename Type = std::size_t>
  LightfieldDimension<Type> get_dimensions() const {
    
    if (!lightfield_dimension) {
      lightfield_dimension =
          std::make_unique<LightfieldDimension<std::size_t>>(this->get_width(), this->get_height(),
              this->get_views_width(), this->get_views_height());
    }
    return LightfieldDimension<Type>(*lightfield_dimension);

    // return LightfieldDimension<Type>(this->get_width(), this->get_height(),
    //     this->get_views_width(), this->get_views_height());
  }

  inline View<T>* operator[](const int i) {
    // auto unique_ptr_to_view = *this->elements_for_2d_access[i];
    // return unique_ptr_to_view.get();
    return (*this->elements_for_2d_access[i]).get();
  }


  /**
   * \brief      Access operator
   *
   * \param[in]  i     index in dimension T of the Lightfield
   *
   * \return     A constant pointer to View<T> that can be used to access data in S dimension
   * \warning    This operator will not check for the availability of the data. Thus it may 
   * cause a segmentation fault. 
   */
  inline const View<T>* operator[](const int i) const {
    // auto unique_ptr_to_view = *this->elements_for_2d_access[i];
    return (*this->elements_for_2d_access[i]).get();
  }


  /**
   * \brief      Creates a new instance of the object with same properties than original.
   *
   * \return     Copy of this object.
   */
  virtual Lightfield* clone() const override {
    return new Lightfield(*this);
  };
};

#endif /* end of include guard: JPLM_LIB_PART2_COMMON_LIGHTFIELD_H__ */