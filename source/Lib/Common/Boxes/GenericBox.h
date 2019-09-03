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

/** \file     GenericBox.h
 *  \brief    
 *  \details  
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-02
 */

#ifndef GENERICBOX_H__
#define GENERICBOX_H__

#include "Box.h"
#include "TBox.h"

template<t_box_id_type box_id, class T, class BaseBox = Box>
class GenericBox : public BaseBox {
 public:
  constexpr static auto id = box_id;
  static_assert(std::is_base_of<Box, BaseBox>::value, "GenericBox must have a class derived from Box as base.");

  GenericBox() : BaseBox(TBox(id), T()) {
  }


  GenericBox(const T& contents) : BaseBox(TBox(id), contents) {
  }


  GenericBox(T&& contents) : BaseBox(TBox(id), std::move(contents)) {
  }


  GenericBox(std::unique_ptr<T>&& contents)
      : BaseBox(TBox(id), std::move(contents)) {
  }


  GenericBox(const GenericBox& other)
      //by avoiding the use of copy constructor from box makes the code run faster
      : Box(other.t_box, other.get_ref_to_contents()) {
  }


  GenericBox(GenericBox&& other) : Box(std::move(other)) {
  }


  virtual ~GenericBox() = default;


  const T& get_ref_to_contents() const noexcept {
    return static_cast<const T&>(*(this->d_box));
  }


  T& get_ref_to_contents() {
    return static_cast<T&>(*(this->d_box));
  }


  T* data() {
    return static_cast<T*>(this->d_box.get());
  }


  const T* data() const {
    return static_cast<T*>(this->d_box.get());
  }


  friend void swap(T& box_a, T& box_b) {
    using std::swap;

    //swap(box_a.t_box, box_b.t_box); unecessary, both will have the same data
    swap(box_a.d_box, box_b.d_box);
  }


  T& operator=(const T& other) {
    if (&other == this)
      return *this;

    T temp{other};
    swap(*this, temp);

    return *this;
  }
};


#endif /* end of include guard: GENERICBOX_H__ */