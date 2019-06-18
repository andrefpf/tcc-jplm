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

/** \file     Generic2DStructure.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-31
 */

#ifndef JPLM_LIB_UTILS_IMAGE_GENERIC2DSTRUCTURE_H__
#define JPLM_LIB_UTILS_IMAGE_GENERIC2DSTRUCTURE_H__

#include <iostream>
#include <memory>  //unique_ptr
#include <utility>  //std::pair
#include <vector>
#include "Lib/Utils/Image/ImageExceptions.h"  //FIXME: change the exceptions to be in namespace Generic2DStructure
#include "Lib/Utils/Image/Raster2DIterator.h"


template<class T>
struct is_unique_ptr : std::false_type {};

template<class T>
struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};


template<typename T>
class Generic2DStructure {
 protected:
  const std::size_t width;
  const std::size_t height;
  const std::size_t number_of_elements = 0;
  std::unique_ptr<T[]> elements;
  std::vector<T*> elements_for_2d_access;


  Generic2DStructure(const std::pair<std::size_t, std::size_t>& size,
      const bool auto_alloc_resources = true)
      : Generic2DStructure<T>(
            std::get<0>(size), std::get<1>(size), auto_alloc_resources) {
  }


  Generic2DStructure(const std::size_t width, const std::size_t height,
      const bool auto_alloc_resources = true)
      : width(width), height(height), number_of_elements(width * height),
        elements(nullptr) {
    if (number_of_elements == 0)
      throw ImageChannelExceptions::InvalidSizeException();
    if (auto_alloc_resources) {
      this->alloc_all_resources();
    }
  }

  //TODO: check if possible to make copy constructor protected
  Generic2DStructure(const Generic2DStructure& other)
      : width(other.width), height(other.height),
        number_of_elements(other.number_of_elements) {
    //FIXME: copy all elements from other
  }


  virtual ~Generic2DStructure() = default;

  //virtual copy constructor idiom
  virtual Generic2DStructure* clone() const = 0;


  void alloc_raster_structure_default_init() {
    elements = std::make_unique<T[]>(number_of_elements);
  }


  void alloc_2d_resource_pointers() {
    elements_for_2d_access.clear();
    elements_for_2d_access.resize(height);
    for (auto i = decltype(height){0}; i < height; ++i) {
      elements_for_2d_access[i] = &elements[i * width];
    }
  }


  virtual void alloc_all_resources() {
    alloc_raster_structure_default_init();
    alloc_2d_resource_pointers();
  }


 public:
  inline bool is_coordinate_valid(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    auto [i, j] = coordinate;
    return i < height && j < width;
  }


  inline bool is_coordinate_valid(const std::size_t i, std::size_t j) const {
    return is_coordinate_valid({i, j});
  }


  virtual void set_element_at(
      const T& element, const std::size_t i, const std::size_t j) {
    if (!is_coordinate_valid(i, j)) {
      throw ImageChannelExceptions::InvalidIndexWriteException();
    }
    if constexpr (is_unique_ptr<T>::value) {
      typedef typename std::remove_reference<decltype(*std::declval<T>())>::type
          ElemType;
      auto element_ptr = element.get();
      //gets the element pointed by the unique_ptr
      this->elements[i * this->width + j] =
          std::make_unique<ElemType>(*element_ptr);
    } else {
      this->elements[i * this->width + j] = element;
    }
  }


  virtual void set_element_at(
      T&& element, const std::size_t i, const std::size_t j) {
    if (!is_coordinate_valid(i, j)) {
      throw ImageChannelExceptions::InvalidIndexWriteException();
    }
    auto data_ptr = elements.get();
    data_ptr[i * this->width + j] = std::move(element);
  }


  virtual void set_element_at(
      const T& element, const std::pair<std::size_t, std::size_t>& coordinate) {
    const auto& [i, j] = coordinate;
    set_element_at(element, i, j);
  }


  virtual void set_element_at(
      T&& element, const std::pair<std::size_t, std::size_t>& coordinate) {
    const auto& [i, j] = coordinate;
    set_element_at(std::move(element), i, j);
  }


  T& get_element_reference_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    const auto& [i, j] = coordinate;
    return get_element_reference_at(i, j);
  }


  void check_for_access_errors(const std::size_t i, const std::size_t j) const {
    if (!elements) {
      //FIXME
      std::cerr << "ptr not set.. " << std::endl;
      exit(2);
    }
    if (!is_coordinate_valid(i, j)) {
      throw ImageChannelExceptions::InvalidIndexReadException();
    }
  }


  T& get_element_reference_at(const std::size_t i, const std::size_t j) const {
    check_for_access_errors(i, j);
    return this->elements[i * this->width + j];
  }


  T get_element_value_at(
      const std::pair<std::size_t, std::size_t>& coordinate) const {
    // std::cout << "getting pixel im 2d structure" << std::endl;
    return get_element_reference_at(coordinate);
  }


  T get_element_value_at(const std::size_t i, const std::size_t j) const {
    return get_element_reference_at(i, j);
  }


  inline T* data() const {
    return this->elements.get();
  }


  inline auto get_width() const noexcept {
    return width;
  }


  inline auto get_height() const noexcept {
    return height;
  }


  inline auto get_dimension() const noexcept {
    return std::make_pair(this->width, this->height);
  }


  bool has_equal_size(const Generic2DStructure<T>& other) const noexcept {
    return (other.width == this->width) && (other.height == this->height);
  }


  auto begin() {
    return Raster2DIterator<T>(this->elements.get());
  }


  auto end() {
    return Raster2DIterator<T>(this->elements.get() + this->number_of_elements);
  }


  auto begin() const noexcept {
    return this->cbegin();
  }


  auto end() const noexcept {
    return this->cend();
  }


  auto cbegin() const noexcept {
    return Raster2DIterator<const T>(this->elements.get());
  }


  auto cend() const noexcept {
    return Raster2DIterator<const T>(
        this->elements.get() + this->number_of_elements);
  }
};

#endif /* end of include guard: JPLM_LIB_UTILS_IMAGE_GENERIC2DSTRUCTURE_H__ */