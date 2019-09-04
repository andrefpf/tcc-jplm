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


template<t_box_id_type box_id, class ContentsClass, class BaseBox = Box>
class GenericBox : public BaseBox {
 public:
  constexpr static auto id = box_id;
  static_assert(std::is_base_of<Box, BaseBox>::value,
      "GenericBox must have a class derived from Box as base.");

  //! \todo check if should use enable_if for GenericBox when using a default contructible (std::is_default_constructible<T>) contents class
  GenericBox() : BaseBox(TBox(id), ContentsClass()) {
  }


  GenericBox(const ContentsClass& contents) : BaseBox(TBox(id), contents) {
  }


  GenericBox(ContentsClass&& contents)
      : BaseBox(TBox(id), std::move(contents)) {
  }


  GenericBox(std::unique_ptr<ContentsClass>&& contents)
      : BaseBox(TBox(id), std::move(contents)) {
  }


  GenericBox(const GenericBox& other) : Box(other) {
  }


  GenericBox(GenericBox&& other) : Box(std::move(other)) {
  }


  virtual ~GenericBox() = default;


  /**
   * \brief      Gets a constant reference to the contents of this box.
   *
   * \return     The constant reference to contents (of the class type T).
   */
  const ContentsClass& get_ref_to_contents() const noexcept override {
    return static_cast<const ContentsClass&>(*(this->d_box));
  }


  ContentsClass& get_ref_to_contents() override {
    return static_cast<ContentsClass&>(*(this->d_box));
  }


  ContentsClass* data() override {
    return static_cast<ContentsClass*>(this->d_box.get());
  }


  const ContentsClass* data() const override {
    return static_cast<ContentsClass*>(this->d_box.get());
  }


  friend void swap(GenericBox& box_a, GenericBox& box_b) {
    using std::swap;
    swap(box_a.t_box, box_b.t_box);  //! \todo check if this is necessary
    swap(box_a.d_box, box_b.d_box);
  }


  GenericBox& operator=(const GenericBox& other) {
    if (&other == this)
      return *this;

    GenericBox temp{other};
    swap(*this, temp);

    return *this;
  }


  GenericBox& operator=(GenericBox&& other) {
    if (*this != other) {
      this->t_box = other.t_box;
      this->d_box = std::move(other.d_box);
    }
    return *this;
  }
};

#endif /* end of include guard: GENERICBOX_H__ */

/*! \page generic_box_info How to create a new box
  \tableofcontents
  This page will introduce briefly how to implement a new box.
  \section sec1 Creating a new box
  Basically, what changes between boxes is their id and content. 
  Given that you already implemented the contents class (which must be derived from DBox), to declare a new Box that holds such contents:

  \code{.cpp}
  using MyNewBox = GenericBox<0x00000000, MyNewContents>;
  \endcode

  For instance, let us use the JpegPlenoSignatureBox:
  
  \snippet Lib/Part1/Common/Boxes/JpegPlenoSignatureContents.h Declaring JpegPlenoSignatureBox

  \section sec2 Accessing the contents of the box

  To get the contents of the box: 

  \code{.cpp}
  auto jpeg_pleno_signature_box = JpegPlenoSignatureBox(); //created the box
  auto& contents_of_the_box = jpeg_pleno_signature_box.get_ref_to_contents();
  //It is also possible to obtain a const reference to the contents:
  const auto& contents_of_the_box = jpeg_pleno_signature_box.get_ref_to_contents();
  \endcode

*/