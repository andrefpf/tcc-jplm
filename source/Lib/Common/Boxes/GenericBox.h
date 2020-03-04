/* The copyright in th\substack{\text{matrix} \\ \text{partition}is software is being made available under the BSD
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
 *  \brief    Generic box definition
 *  \details  This templated class adopts 3 template parameters: box_id,
 *            ContentsClass, and BaseBox, that are used to specify a box.
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-09-02
 */

#ifndef GENERICBOX_H__
#define GENERICBOX_H__

#include "Box.h"
#include "TBox.h"


/**
 * \brief      Templated class for creating a specified box using this generic box definition.
 *
 * \tparam     box_id         The id representing the type of this box. It will be the value stored in TBox
 * \tparam     ContentsClass  The class that represents the contents (DBox) of the Box with box_id
 * \tparam     BaseBox        A Box class used as the direct base from which this GenericBox class inherits. 
 *                            The default is to inherit directly from Box. 
 *                            However, to support other abstractions, another class that inherits from Box can be used.
 *                            
 */
template<t_box_id_type box_id, class ContentsClass, class BaseBox = Box>
class GenericBox : public BaseBox {
 public:
  constexpr static auto id = box_id;
  static_assert(std::is_base_of<Box, BaseBox>::value,
      "GenericBox must have a class derived from Box as base.");

  /**
   * \brief      Default constructor of this object. 
   * \details    This default constructor will only exists if ContentsClass also provides a default constructor (std::is_default_constructible<ContentsClass>).
   *             Otherwise the code won't compile.
   */
  GenericBox() : BaseBox(TBox(id), ContentsClass()) {
    std::cout << "GenericBox() : BaseBox(TBox(id), ContentsClass())"
              << std::endl;
  }


  /**
   * \brief      Constructs the Box object copying the contents passed as parameter.
   *
   * \param[in]  contents  The contents
   */
  GenericBox(const ContentsClass& contents) : BaseBox(TBox(id), contents) {
    std::cout << "GenericBox(const ContentsClass& contents) : "
                 "BaseBox(TBox(id), contents)"
              << std::endl;
  }


  /**
   * \brief      Constructs the Box object moving the contents passed as parameter into this box contents.
   *
   * \param      contents  The contents
   */
  GenericBox(ContentsClass&& contents)
      : BaseBox(TBox(id), std::move(contents)) {
    std::cout << "GenericBox(ContentsClass&& contents)" << std::endl;
  }


  /**
   * \brief      Constructs the Box object moving the contents passed as parameter.
   *
   * \param      contents  The contents
   */
  GenericBox(std::unique_ptr<ContentsClass>&& contents)
      : BaseBox(TBox(id), std::move(contents)) {
    std::cout << "GenericBox(std::unique_ptr<ContentsClass>&& contents)"
              << std::endl;
  }


  /**
   * \brief      Copy constructor
   *
   * \param[in]  other  The other box that wil be copied to this box.
   */
  GenericBox(const GenericBox& other) : BaseBox(other) {
    std::cout << "GenericBox(const GenericBox& other) : BaseBox(other)"
              << std::endl;
  }


  /**
   * \brief      Move constructor
   *
   * \param      other  The other box that will have its contents moved to the newly created box.
   * 
   * \details    The actual move only happens in BaseBox. 
   *             Thus, to use this constructor the BaseBox must provide a move constructor.
   */
  GenericBox(GenericBox&& other) : BaseBox(std::move(other)) {
    std::cout << "GenericBox(GenericBox&& other) : BaseBox(std::move(other))"
              << std::endl;
  }


  /**
   * \brief      Destroys the object.
   */
  virtual ~GenericBox() = default;


  /**
   * \brief      Gets a constant reference to the contents of this box.
   *
   * \return     The constant reference to contents (of the class type ContentsClass).
   */
  const ContentsClass& get_ref_to_contents() const noexcept override {
    return static_cast<const ContentsClass&>(*(this->d_box));
  }


  /**
   * \brief      Gets a non constant reference to the contents of this box.
   *
   * \return     The reference to contents (of the class type ContentsClass)..
   */
  ContentsClass& get_ref_to_contents() override {
    return static_cast<ContentsClass&>(*(this->d_box));
  }


  /**
   * \brief      Gets a non constant raw pointer to the contents of this box.
   *
   * \return     The non constant raw pointer to contents (of the class type ContentsClass).
   */
  ContentsClass* data() override {
    return static_cast<ContentsClass*>(this->d_box.get());
  }


  /**
   * \brief      Gets a constant raw pointer to the contents of this box.
   *
   * \return     The constant raw pointer to contents (of the class type ContentsClass)..
   */
  const ContentsClass* data() const override {
    return static_cast<ContentsClass*>(this->d_box.get());
  }


  /**
   * \brief      Swaps the contents of box_a for box_b and of box_b for box_a
   *
   * \param[in,out]      box_a  The box a
   * \param[in,out]      box_b  The box b
   */
  friend void swap(GenericBox& box_a, GenericBox& box_b) {
    using std::swap;
    swap(box_a.t_box,
        box_b.t_box);  //! \todo check if swaping the box types is necessary
    swap(box_a.d_box, box_b.d_box);
  }


  /**
   * \brief      Assigment operator
   *
   * \param[in]  other  The other box that will be copied into this box
   *
   * \return     A reference to this box after copying other box contents into this
   * 
   * \details    Using the copy/swap idiom
   */
  GenericBox& operator=(const GenericBox& other) {
    if (&other == this)
      return *this;

    GenericBox temp{other};
    swap(*this, temp);

    return *this;
  }


  /**
   * \brief      Move assigment operator
   *
   * \param      other  The other box which contents will be moved to this box
   *
   * \return     A reference to this box containing the resources of the other box.
   */
  GenericBox& operator=(GenericBox&& other) {
    std::cout << "move assigment" << std::endl;
    if (*this != other) {
      this->t_box = other.t_box;
      this->d_box = std::move(other.d_box);
    }
    return *this;
  }
};

#endif /* end of include guard: GENERICBOX_H__ */

/*! \page a_generic_box_info How to create a new box
  \tableofcontents
  This page will introduce briefly how to implement a new box.
  \section creating_box Creating a new box
  Basically, what changes between different boxes that are specified in the standard are their id and content. 
  Given that you already implemented the contents class (see \ref data_box_info), to declare a new Box that holds such contents:

  \code{.cpp}
  using MyNewBox = GenericBox<0x00000000, MyNewContents>;
  \endcode

  For instance, let us use as example the \link JpegPlenoSignatureBox \endlink. Its definition is as simple as this: 
  
  \snippet Lib/Part1/Common/Boxes/JpegPlenoSignatureBox.h Declaring JpegPlenoSignatureBox

  Also, it is important to define new boxes in separated/distinct files to keep the source files organized. 
  Thus, when a new box must be included in other part of the code, it will be easier to find. 

  \section accessing_box Accessing the contents of the box

  To get the contents of the box: 

  \code{.cpp}
  auto jpeg_pleno_signature_box = JpegPlenoSignatureBox(); //created the box
  auto& contents_of_the_box = jpeg_pleno_signature_box.get_ref_to_contents();
  //It is also possible to obtain a const reference to the contents:
  const auto& contents_of_the_box = jpeg_pleno_signature_box.get_ref_to_contents();
  \endcode

*/